#include <math.h>
#include <stdio.h>

#include "array_nearest_neighbor.h"


Point arrayNearestNeighbor(Point *points, size_t pointsLength, Point point) {
    Point closestPoint = {INFINITY, INFINITY};
    for (size_t i = 0; i < pointsLength; i++) {
        if (calcSquaredDistance(point, points[i]) < calcSquaredDistance(point, closestPoint)) {
            closestPoint = points[i];
        }
    }
    return closestPoint;
}

void printPointArray(Point *points, size_t pointsLength) {
    printf("points:\n");
    for (size_t i = 0; i < pointsLength; i++) {
        printf("%.2f, %.2f; ", points[i].x, points[i].y);
    }
    printf("\n");
}

