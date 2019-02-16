#ifndef AREA_H
#define AREA_H

#include <QVector>
#include <QPoint>
#include <algorithm>
#include <math.h>
#include <limits>


struct HeightCoordinate
{
    HeightCoordinate() = default;
    HeightCoordinate(int x, int y, float height): x(x), y(y), height(height) {}
    HeightCoordinate(QPoint point, float height): x(point.x()), y(point.y()), height(height) {}

    HeightCoordinate& operator=(const HeightCoordinate& obj)
    {
        if (this == &obj)
        {
            return *this;
        }

        x = obj.x;
        y = obj.y;
        height = obj.height;
        return *this;
    }

    bool operator==(const HeightCoordinate& rvalue)
    {
        return x == rvalue.x and  y == rvalue.y and height == rvalue.height;
    }

    bool operator!=(const HeightCoordinate& rvalue)
    {
        return *this == rvalue;
    }

    int x, y;
    float height;
};

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

    void addPoint(const HeightCoordinate& point)
    {
        points.append(point);
    }

    HeightCoordinate getCenterPoint();

    qint32 id;
    QVector<HeightCoordinate> points;

private:
    HeightCoordinate centerCoord;
};


#endif // AREA_H
