#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "kd_tree.h"

Point arrayNearestNeighbor(Point *points, size_t pointsLength, Point point) {
    Point closestPoint = {INFINITY, INFINITY};
    for (size_t i = 0; i < pointsLength; i++) {
        if (calcSquaredDistance(point, points[i]) < calcSquaredDistance(point, closestPoint)) {
            closestPoint = points[i];
        }
    }
    return closestPoint;
}

int main() {
    srand(time(0));

    const int32_t maxCoord = 1000;
    const size_t pointsLength = 1000000;
    
    Point* points = (Point*)malloc(sizeof(Point) * pointsLength);
    for (size_t i = 0; i < pointsLength; i++) {
        points[i].x = (double)(rand() % (200 * maxCoord + 1) - maxCoord * 100) / 100.0;
        points[i].y = (double)(rand() % (200 * maxCoord + 1) - maxCoord * 100) / 100.0;
    }
    
    //printf("points:\n");
    //for (size_t i = 0; i < pointsLength; i++) {
    //    printf("%.2f, %.2f; ", points[i].x, points[i].y);
    //}
    //printf("\n");

    struct timespec start, finish;
    timespec_get(&start, TIME_UTC);
    // #define __INSERT__
    #ifdef __INSERT__
    KdTree* kdTree = kdInitEmpty();
    for (size_t i = 0; i < pointsLength; i++) {
        kdInsert(kdTree, points[i]);
    }
    #else
    KdTree* kdTree = kdInit(points, pointsLength);
    #endif
    timespec_get(&finish, TIME_UTC);
    double elapsed_ms = (double)(finish.tv_sec - start.tv_sec) * 1000.0 +
        (double)(finish.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Elapsed time init kd tree: %.5f ms\n", elapsed_ms);
    printf("Depth of kd tree: %zu\n", kdGetDepth(kdTree));
    //kdPrint(kdTree);

    Point searchedPoint = {0.5, 0.5};

    timespec_get(&start, TIME_UTC);
    Point point = kdNearestNeighbor(kdTree, searchedPoint);
    timespec_get(&finish, TIME_UTC);
    double distance = sqrt(point.x * point.x + point.y * point.y);
    printf("Nearest neighbor is {%.2f, %.2f} with distance %.2f\n", point.x, point.y, distance);
    elapsed_ms = (double)(finish.tv_sec - start.tv_sec) * 1000.0 +
        (double)(finish.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Elapsed time searching nearest neighbor kd tree: %.5f ms\n", elapsed_ms);

    timespec_get(&start, TIME_UTC);
    point = arrayNearestNeighbor(points, pointsLength, searchedPoint);
    timespec_get(&finish, TIME_UTC);
    distance = sqrt(point.x * point.x + point.y * point.y);
    printf("Nearest neighbor with array is {%.2f, %.2f} with distance %.2f\n", point.x, point.y, distance);
    elapsed_ms = (double)(finish.tv_sec - start.tv_sec) * 1000.0 +
        (double)(finish.tv_nsec - start.tv_nsec) / 1000000.0;
    printf("Elapsed time searching nearest neighbor array: %.5f ms\n", elapsed_ms);

    kdDeinit(kdTree);
    free(points);
}
