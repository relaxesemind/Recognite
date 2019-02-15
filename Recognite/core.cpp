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

   QImage image(sizeX,sizeY,QImage::Format_RGB32);

   for (int i = 0; i < sizeY; ++i)
   {
       for (int j = 0; j < sizeX; ++j)
       {
            int comp = static_cast<int>(
                                        model.matrix[i][j] * 255.f /
                                        static_cast<float>(std::abs(max))
                                        );
            QRgb value = qRgb(comp,comp,comp);
            image.setPixel(i,j,value);
       }
   }

   return image;
}

void Core::setRange(int min, int max)
{
    rangeMax = max;
    rangeMin = min;
}

inline bool Core::inRange(qint32 x, qint32 y, const QVector<QVector<float>>& matrix)
{
    int comp = static_cast<int>(
                                matrix[y][x] * 255.f /
                                static_cast<float>(std::abs(7.f))
                                );

    return (comp <= rangeMax) and (comp >= rangeMin);
}

void Core::fill(const QVector<QVector<float>>& matrix, QVector<QVector<qint32>>& V, qint32 x, qint32 y, qint32 L)
{
    if (matrix.isEmpty())
    {
        return;
    }

    QPoint t;
    QStack<QPoint> depth;
    depth.push(QPoint(x,y));
    const qint32 w = matrix.first().size();
    const qint32 h = matrix.size();

    while (!depth.empty())
    {
      t = depth.pop();
      qint32 x = t.rx();
      qint32 y = t.ry();
      V[y][x] = L; // filling.

      if((x + 1 < w)&&(inRange(x + 1,y,matrix))&&(V[y][x + 1] == 0))
      {
          depth.push(QPoint(x + 1,y));
      }
      if((x - 1> -1)&&(inRange(x - 1,y,matrix))&&(V[y][x - 1] == 0))
      {
          depth.push(QPoint(x - 1,y));
      }
      if((y + 1< h)&&(inRange(x,y + 1,matrix))&&(V[y + 1][x] == 0))
      {
          depth.push(QPoint(x,y + 1));
      }
      if((y - 1> -1)&&(inRange(x,y - 1,matrix))&&(V[y - 1][x] == 0))
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

    InputModel model = parser.inputModelFromFile(path);

    if (model.matrix.isEmpty())
    {
        return QImage();
    }

    QVector<QVector<float>>& source = model.matrix;

    qint32 const _h = source.size();
    qint32 const _w = source.first().size();
    qint32 L = 1; // starting id value

    QVector<QVector<qint32>> labels(_h,QVector<qint32>(_w,0));

 //labeling

    for(qint32 y = 0; y < _h; ++y)
      for(qint32 x = 0; x < _w; ++x)
      {
        if((!labels[y][x])&&(inRange(x,y,source)))
        {
          fill(source,labels,x,y,L++); //very fast!
        }
      }

//form objects

//    const qint32 size = --L; // size = num of objects
//    QVector<Area> objects(size);

//    for (qint32 i = 0; i < size; ++i)
//    {
//        objects[i] = Area(i);
//    }

//    if ((size > 0)&&(_h > 2)&&(_w > 2))
//    {

//    for(int y = 1; y < _h - 1; ++y)//general case
//      for(int x = 1; x < _w - 1; ++x)//general case
//      {
//          auto id = labels[y][x];
//          if ((id > 0)&&(id < size + 1))
//          {
//            objects[id - 1].addPoint(QPoint(x,y));
//          }
//      }

//    for(int x = 1; x < _w - 1; ++x)//top case
//     {
//        auto id = labels[0][x];
//        if ((id > 0)&&(id < size + 1))
//        {
//           objects[id - 1].addPoint(QPoint(x,0));
//        }
//     }
//    for(int x = 1; x < _w - 1; ++x)//bottom case
//     {
//        auto id = labels[_h - 1][x];
//        if ((id > 0)&&(id < size + 1))
//        {
//           objects[id - 1].addPoint(QPoint(x,_h - 1));
//        }
//     }
//    for(int y = 0; y < _h; ++y)//left case
//     {
//         auto id = labels[y][0];
//         if ((id > 0)&&(id < size + 1))
//         {
//            objects[id - 1].addPoint(QPoint(0,y));
//         }
//     }
//    for(int y = 0; y < _h; ++y)//right case
//     {
//         auto id = labels[y][_w - 1];
//         if ((id > 0)&&(id < size + 1))
//         {
//            objects[id - 1].addPoint(QPoint(_w - 1,y));
//         }
//     }
//   }
    QImage image(_w,_h,QImage::Format_RGB32);

    for (int i = 0; i < _h; ++i)
    {
        for (int j = 0; j < _w; ++j)
        {
             if (labels[i][j])
             {
                image.setPixel(i,j,qRgb(255,255,255));
             }
             else
             {
                 image.setPixel(i,j,qRgb(0,0,0));
             }
        }
    }
    return image;
}













