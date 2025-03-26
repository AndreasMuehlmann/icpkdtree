#include "point.h"

double getDimension(Point* point, size_t dimension) {
    if (dimension == 0) {
        return point->x;
    }
    return point->y;
}

double calcSquaredDistance(Point point1, Point point2) {
    double xDiff = point2.x - point1.x;
    double yDiff = point2.y - point1.x;
    return xDiff * xDiff + yDiff * yDiff;
}

