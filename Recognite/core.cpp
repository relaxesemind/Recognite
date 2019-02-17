#include "core.h"


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

   float max = std::numeric_limits<float>::min();
   float min = std::numeric_limits<float>::max();

   int sizeY = model.sizeY();
   int sizeX = model.sizeX();

   model.foreachHeight([&max,&min](float& value)
   {
       max = value > max ? value : max;
       min = value < min ? value : min;
   });

   model.foreachHeight([min](float& value)
   {
       value += std::abs(min);
   });

   max += std::abs(min);
   min += std::abs(min);

   model.min = min;
   model.max = max;

   QImage image(sizeX,sizeY,QImage::Format_RGB32);

   for (int i = 0; i < sizeY; ++i)
   {
       for (int j = 0; j < sizeX; ++j)
       {
            int comp = model.colorOfHeight(j,i);
            image.setPixel(i,j,qRgb(comp,comp,comp));
       }
   }

   models.append(model);
   return image;
}

void Core::setRange(int min, int max)
{
    rangeMin = min;
    rangeMax = max;
}

std::pair<float, float> Core::findAbsoluteMaxMinHeights()
{
    auto& objects = StaticModel::shared().objectsMap;
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    std::for_each(objects.begin(),objects.end(),[&min, &max](QVector<Area>& singleImageObjects)
    {
        std::for_each(singleImageObjects.begin(),singleImageObjects.end(),[&min,&max](Area& area)
        {
            float center = area.getCenterPoint().height;
            max = center > max ? center : max;
            min = center < min ? center : min;
        });
    });

    return std::make_pair(max,min);
}

void Core::calculateFrequencies(int numOfColumn)
{
    if (numOfColumn <= 0)
    {
        return;
    }

    qDebug() << "start calculateFrequencies";

    auto& objects = StaticModel::shared().objectsMap;
    auto& frequencies = StaticModel::shared().frequencies;
    float max = StaticModel::shared().absoluteMAXheight;
    float min = StaticModel::shared().absoluteMINheight;


    float singleInterval = (max - min) / numOfColumn;

    qDebug() << "max = " << max << " min = " << min;
    qDebug() << "singleInterval = " << singleInterval;
    qDebug() << "objects num = " << objects.size();

    frequencies.clear();
    frequencies.resize(numOfColumn);

    std::for_each(objects.begin(),objects.end(),[&](QVector<Area>& singleImageObjects)
    {
        std::for_each(singleImageObjects.begin(),singleImageObjects.end(),[&](Area& object)
        {
            float center = object.getCenterPoint().height;

            int column = 0;

            while(center > min + singleInterval * (column + 1) and min + singleInterval * (column + 1) <= max)
            {
                ++column;
            }
            if (column < frequencies.size() - 1)
            {

               ++frequencies[column];
            }

        });
    });

    qDebug() << "end calculateFrequencies";
}

QVector<QPointF> Core::calcPointsForGraph()
{
    auto& frequencies = StaticModel::shared().frequencies;
    QVector<QPointF> result(frequencies.size());

    repeat(i,frequencies.size())
    {
        result[i] = QPointF(i,frequencies[i]);
    }

    return result;
}

inline bool Core::inRange(qint32 x, qint32 y, const InputModel& model)
{
    int comp = model.colorOfHeight(x,y);
    return  (comp <= rangeMax) and (comp >= rangeMin);
}

void Core::fill(const InputModel &model, QVector<QVector<qint32>>& V, qint32 x, qint32 y, qint32 L)
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

    for (int i = 0;i < models.count();++i)
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

    StaticModel::shared().objectsMap.insert(model.path,std::move(objects));

//form image

    QImage image(_w,_h,QImage::Format_RGB32);

    for (int i = 0; i < _h; ++i)
    {
        for (int j = 0; j < _w; ++j)
        {
             if (labels[i][j])
             {
                image.setPixel(i,j,Consts::whiteRgb);
             }
             else
             {
                 image.setPixel(i,j,Consts::blackRgb);
             }
        }
    }

    return image;
}













