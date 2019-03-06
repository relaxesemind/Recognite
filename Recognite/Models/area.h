#ifndef AREA_H
#define AREA_H

#include <QVector>
#include <QPoint>
#include <algorithm>
#include <math.h>
#include <QDebug>
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

    bool operator<(const HeightCoordinate& rvalue) const
    {
        return height < rvalue.height;
    }

    bool operator==(const HeightCoordinate& rvalue) const
    {
        return x == rvalue.x and  y == rvalue.y and height == rvalue.height;
    }

    bool operator!=(const HeightCoordinate& rvalue) const
    {
        return !(*this == rvalue);
    }

    int x, y;
    float height;
};

struct Area
{
    Area() = default;
    explicit Area(qint32 number): id(number), points(0), maxHeight(HeightCoordinate()) {}

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
    HeightCoordinate getMaxHeight(bool force = true);

    qint32 id;
    QVector<HeightCoordinate> points;

private:
    HeightCoordinate centerCoord;
    HeightCoordinate maxHeight;
};


#endif // AREA_H
