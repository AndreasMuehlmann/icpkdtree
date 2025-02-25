#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "kd_tree.h"

int main() {
    srand(time(0));

    const int32_t maxCoord = 10.0;
    const size_t pointsLength = 10;
    
    Point points[pointsLength];
    for (size_t i = 0; i < pointsLength; i++) {
        points[i].x = (double)(rand() % (200 * maxCoord + 1) - maxCoord * 100) / 100.0;
        points[i].y = (double)(rand() % (200 * maxCoord + 1) - maxCoord * 100) / 100.0;
    }
    KdTree kdTree;
    kdInit(&kdTree, points, pointsLength);
}
