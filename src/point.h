#ifndef __POINT__
#define __POINT__

#include <stddef.h>

typedef struct {
    double x;
    double y;
} Point;


double getDimension(Point* point, size_t dimension);
double calcSquaredDistance(Point point1, Point point2);

#endif
