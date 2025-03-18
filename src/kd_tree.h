#ifndef __KD_TREE__
#define __KD_TREE__

#include <stdlib.h>

typedef struct Node Node;

typedef struct {
    double x;
    double y;
} Point;

struct Node {
    Point point;
    Node *leftChild;
    Node *rightChild;
};

typedef struct {
    Node *root;
} KdTree;

KdTree* kdInit(Point* points, size_t pointsLength);
void kdDeinit(KdTree* kdTree);
void kdPrint(KdTree* kdTree);
ssize_t binarySearch(Point *points, size_t pointsLength, double searched, double (*getCoordinate)(Point));

#endif 
