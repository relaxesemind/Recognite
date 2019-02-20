#include "area.h"

HeightCoordinate Area::getCenterPoint()
{
    if (points.isEmpty())
    {
        return HeightCoordinate();
    }

    if (centerCoord == HeightCoordinate())
    {
        int minX = std::numeric_limits<int>::max();
        int maxX = std::numeric_limits<int>::min();
        int minY = minX;
        int maxY = maxX;

        for (int i = 0; i < points.count(); ++i)
        {
            int x = points[i].x;
            int y = points[i].y;

            maxX = x > maxX ? x : maxX;
            minX = x < minX ? x : minX;

            maxY = y > maxY ? y : maxY;
            minY = y < minY ? y : minY;
        }

        int averageX = (maxX - minX) / 2;
        int averageY = (maxY - minY) / 2;
        float minR = std::numeric_limits<float>::max();
        int index = 0;

        for (int i = 0; i < points.count(); ++i)
        {
            int distanceX = std::abs(points[i].x - averageX);
            int distanceY = std::abs(points[i].y - averageY);
            float radius = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

            if (radius < minR)
            {
                minR = radius;
                index = i;
            }
        }
        centerCoord = points[index];
    }

    return centerCoord;
}

float Area::getMaxHeight(bool force)
{
    if (points.isEmpty())
    {
        return 0.f;
    }

    if (maxHeight == 0.f or force)
    {
        maxHeight = (*std::max_element(points.begin(),points.end())).height;
    }

    return maxHeight;
}
