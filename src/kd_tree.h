#ifndef __KD_TREE__
#define __KD_TREE__

#include <stddef.h>

typedef struct Node Node;

typedef struct {
    double x;
    double y;
} Point;

struct Node {
    Point point;
    Node *children;
    size_t childLength;
};

typedef struct {
    Node *root;
} KdTree;

void kdInit(KdTree* kdTree, Point* points, size_t pointsLength);

#endif 
