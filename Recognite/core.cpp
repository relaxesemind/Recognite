#include "core.h"

inline QRgb grayScalePixel(int value)
{
    return qRgb(value,value,value);
}

QImage Core::imageFromTxtFile(const QString &path)
{
   if (path.isEmpty())
   {
       return QImage();
   }

   InputModel model = parser.inputModelFromFile(path);

   if (!model.isValid())
   {
       return QImage();
   }

   auto& inputModels = StaticModel::shared().inputModels;

   int sizeY = model.sizeY();
   int sizeX = model.sizeX();

   QImage image(sizeX,sizeY,QImage::Format_RGB32);

   for (int i = 0; i < sizeY; ++i)
   {
       for (int j = 0; j < sizeX; ++j)
       {
            image.setPixel(j,i,grayScalePixel(model.colorOfHeight(j,i)));
       }
   }

   inputModels.append(model);
   return image;
}

void Core::setRange(float min, float max)
{
    minFromUI = min;
    maxFromUI = max;
}

std::pair<float, float> Core::findAbsoluteMaxMinHeights()
{
    auto& models = StaticModel::shared().inputModels;

    float max = std::numeric_limits<float>::min();
    float min = std::numeric_limits<float>::max();

    for (InputModel& model: models)
    {
        auto pair = model.getMaxMin();
        float localMax = pair.first;
        float localMin = pair.second;

        max = localMax > max ? localMax : max;
        min = localMin < min ? localMin : min;
    }

    StaticModel::shared().absoluteMAXheight = max;
    StaticModel::shared().absoluteMINheight = min;

    return std::make_pair(max,min);
}

void Core::calculateFrequencies(const QString& seriaPath, int numOfColumn)
{
    if (numOfColumn <= 0 || seriaPath.isEmpty())
    {
        return;
    }

    auto it = StaticModel::shared().frequencies.find(seriaPath);

    if (it == StaticModel::shared().frequencies.end())
    {
        it = StaticModel::shared().frequencies.insert(seriaPath,{});
    }

    auto& frequencies = *it;
    auto& objects = StaticModel::shared().objectsMap;

    float max = StaticModel::shared().absoluteMAXheight;
    float min = StaticModel::shared().absoluteMINheight;


    float singleInterval = std::abs(max - min) / numOfColumn;

    qDebug() << "max = " << max << " min = " << min;
    qDebug() << "singleInterval = " << singleInterval;
    qDebug() << "objects num = " << objects.size();

    frequencies.clear();
    frequencies.resize(numOfColumn);

    StaticModel::shared().foreachArea([&frequencies, min, max, singleInterval](Area& object)
    {
        float height = object.getMaxHeight().height;
        int column = 0;

        while(height > min + singleInterval * (column + 1) and min + singleInterval * (column + 1) <= max)
        {
            ++column;
        }

        if (column < frequencies.size() - 1)
        {

           ++frequencies[column];
        }

    });
}

void Core::calculateFrequenciesWithInterval(const QString &seriaPath, float interval)
{
    if (interval < 0.0001f)
    {
        return;
    }

    float max = StaticModel::shared().absoluteMAXheight;
    float min = StaticModel::shared().absoluteMINheight;
    int columns = static_cast<int>((max - min) / interval);
    this->calculateFrequencies(seriaPath,columns);
}

QMap<QString, QVector<QPointF>> Core::calcPointsForGraph()
{
    QMap<QString, QVector<QPointF>> map;
    auto & freqM = StaticModel::shared().frequencies;

    for (auto it = freqM.begin(); it != freqM.end(); ++it)
    {
        auto data = it.value();
        QVector<QPointF> points(data.size());

        repeat(i,data.size())
        {
            points[i] = QPointF(i,data[i]);
        }
        map.insert(it.key(),points);
    }

    return map;
}

void Core::setMinObjectSize(int value)
{
    minObjectSize = value;
}

void Core::getTrueHeights(const InputModel& model, QVector<Area>& objects)
{
    std::for_each(objects.begin(),objects.end(),[model](Area& area)
    {
        HeightCoordinate height(area.getMaxHeight());
        int traverseSize = area.points.size();

        traverseSize = traverseSize > 40 ? 3 * std::sqrt(traverseSize) : traverseSize;

        //y = y0 ;x = x0

        int i = 0;
        const int step = SettingsStorage::shared().traverseWalkStep;
        const float stability = SettingsStorage::shared().derivativeStability;
        bool traverseIsFlat = false;
        int confidentialCounter = 0;
        const int x1 = height.x;
        const int y1 = height.y;

        int xr = x1, xl = x1;
        int yt = y1, yb = y1;

        auto derivative = [model,step](QPoint p1, QPoint p2)->float
        {
            return (model.matrix[p2.y()][p2.x()] - model.matrix[p1.y()][p1.x()]) / step;
        };

        while (++i < traverseSize)
        {
            bool flatter = false;

            if (model.isSafelyIndexes(xr + step, y1))
            {
                float derivativeRight = derivative(QPoint(xr,y1),QPoint(xr + step,y1));
                xr += step;

                flatter = std::abs(derivativeRight) < stability;
            }

            if (model.isSafelyIndexes(xl - step, y1))
            {
                float derivativeLeft = derivative(QPoint(xl,y1),QPoint(xl - step,y1));
                xl -= step;

                flatter = std::abs(derivativeLeft) < stability;
            }

            if (model.isSafelyIndexes(x1,yt - step))
            {
                float derivativeTop = derivative(QPoint(x1,yt),QPoint(x1,yt - step));
                yt -= step;

                flatter = std::abs(derivativeTop) < stability;
            }

            if (model.isSafelyIndexes(x1,yb + step))
            {
                float derivativeBottom = derivative(QPoint(x1,yb), QPoint(x1,yb + step));
                yb += step;

                flatter = std::abs(derivativeBottom) < stability;
            }


            if (flatter)
            {
                ++confidentialCounter;
            }

            if (confidentialCounter > 2)
            {
                traverseIsFlat = true;
                break;
            }
        }

        if (!traverseIsFlat)
        {
            return ;
        }

        float h1 = model.matrix[y1][xr];
        float h2 = model.matrix[y1][xl];
        float h3 = model.matrix[yt][x1];
        float h4 = model.matrix[yb][x1];
        float h = (h1 + h2 + h3 + h4) / 4;

        qDebug() << "traverseIsFlat! h = " << h;
//        std::cout << "walk is flat! (" << height.x << "," << height.y << ") h = " << h << std::endl;

        std::for_each(area.points.begin(),area.points.end(),[h](HeightCoordinate& coord)
        {
            coord.height -= h;
        });
    });
}

inline bool Core::inRange(qint32 x, qint32 y, const InputModel& model)
{
    float height = model.matrix[y][x];
    return  height <= maxFromUI and height >= minFromUI;
}

void Core::fill(const InputModel& model, QVector<QVector<qint32>>& V, qint32 x, qint32 y, qint32 L)
{
    if (!model.isValid())
    {
        return;
    }

    QPoint t;
    QStack<QPoint> depth;
    depth.push(QPoint(x,y));
    const qint32 w = model.sizeX();
    const qint32 h = model.sizeY();

    while (!depth.empty())
    {
      t = depth.pop();
      qint32 x = t.x();
      qint32 y = t.y();
      V[y][x] = L; // filling.

      if((x + 1 < w)&&(inRange(x + 1,y,model))&&(V[y][x + 1] == 0))
      {
          depth.push(QPoint(x + 1,y));
      }
      if((x - 1> -1)&&(inRange(x - 1,y,model))&&(V[y][x - 1] == 0))
      {
          depth.push(QPoint(x - 1,y));
      }
      if((y + 1< h)&&(inRange(x,y + 1,model))&&(V[y + 1][x] == 0))
      {
          depth.push(QPoint(x,y + 1));
      }
      if((y - 1> -1)&&(inRange(x,y - 1,model))&&(V[y - 1][x] == 0))
      {
          depth.push(QPoint(x,y - 1));
      }
    }
}

QImage Core::binImageFromTxtFile(const QString &path)
{
    if (path.isEmpty())
    {
        return QImage();
    }

    int id = 0;
    auto& models = StaticModel::shared().inputModels;

    for (int i = 0;i < models.count(); ++i)
    {
        if (models[i].path == path)
        {
            id = i;
            break;
        }
    }

    InputModel& model = models[id];

    if (!model.isValid())
    {
        return QImage();
    }

    qint32 const _h = model.sizeY();
    qint32 const _w = model.sizeX();
    qint32 L = 1; // starting id value

    QVector<QVector<qint32>> labels(_h,QVector<qint32>(_w,0));

 //labeling

    for(qint32 y = 0; y < _h; ++y)
      for(qint32 x = 0; x < _w; ++x)
      {
        if((!labels[y][x])&&(inRange(x,y,model)))
        {
           fill(model,labels,x,y,L++); //very fast!
        }
      }

//form objects

    const qint32 size = --L; // size == num of objects
    QVector<Area> objects(size, Area());

       if ((size > 0) && (_h > 2) && (_w > 2))
       {
           for(int y = 0; y < _h; ++y)
             for(int x = 0; x < _w; ++x)
             {
                 qint32 id = labels[y][x];
                 if ((id > 0) && (id < size + 1))
                 {
                   objects[id - 1].addPoint(HeightCoordinate(x,y,model.matrix[y][x]));
                 }
             }
        }

    int min_objSize = minObjectSize;
    int sizeBefore = objects.size();

    objects.erase(
    std::remove_if(objects.begin(),objects.end(),[min_objSize](Area& obj){return obj.points.count() < min_objSize;}),
                   objects.end()
            );

    QVector<QVector<bool>> updatedLabels(_h,QVector<bool>(_w,false));

    std::for_each(objects.begin(),objects.end(),[&updatedLabels](Area& area){
        std::for_each(area.points.begin(),area.points.end(),[&updatedLabels](HeightCoordinate& coord){
            updatedLabels[coord.y][coord.x] = true;
        });
    });


    qDebug() <<"size before = " << sizeBefore << " after = " << objects.size();

    StaticModel::shared().objectsMap.insert(model.path,std::move(objects));

//form image

    QImage image(_w,_h,QImage::Format_RGB32);

    for (int i = 0; i < _h; ++i)
    {
        for (int j = 0; j < _w; ++j)
        {
            image.setPixel(j,i,updatedLabels[i][j] ? Consts::whiteRgb : Consts::blackRgb);
        }
    }

    return image;
}













