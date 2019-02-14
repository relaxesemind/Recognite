#ifndef AREA_H
#define AREA_H

#include <QVector>
#include <QPoint>

struct Area
{
    Area() = default;
    explicit Area(qint32 number): id(number), points(0) {}

    Area& operator=(const Area& obj)
    {
        if (this == &obj)
        {
            return *this;
        }

        id = obj.id;
        points = obj.points;
        return *this;
    }

    void addPoint(QPoint point)
    {
        points.append(point);
    }

    qint32 id;
    QVector<QPoint> points;
};


#endif // AREA_H
