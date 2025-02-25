#include "kd_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void calcCenter(Point* points, size_t pointsLength, Point* median) {
    for (size_t i = 0; i < pointsLength; i++) {
        median->x += points[i].x;
        median->y += points[i].y;
    }
}

int compareX(const void* a, const void* b) {
    double value1 = ((Point*)a)->x;
    double value2 = ((Point*)b)->x;
    if (value1 > value2) {
        return 1;
    } else if (value1 < value2) {
        return -1;
    } else {
        return 0;
    }
}

int compareY(const void* a, const void* b) {
    double value1 = ((Point*)a)->y;
    double value2 = ((Point*)b)->y;
    if (value1 > value2) {
        return 1;
    } else if (value1 < value2) {
        return -1;
    } else {
        return 0;
    }
}

void kdInit(KdTree* kdTree, Point* points, size_t pointsLength) {
    qsort(points, pointsLength, sizeof(Point), compareX);

    Point* pointsSortedY = (Point*)malloc(sizeof(Point) * pointsLength);
    memcpy(pointsSortedY, points, sizeof(Point) * pointsLength);

    qsort(pointsSortedY, pointsLength, sizeof(Point), compareY);

    free(pointsSortedY);
}
