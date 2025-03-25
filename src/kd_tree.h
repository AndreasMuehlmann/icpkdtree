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


KdTree* kdInitBalanced(Point* points, size_t pointsLength);
KdTree* kdInit(Point* points, size_t pointsLength);
KdTree* kdInitEmpty();
void kdDeinit(KdTree* kdTree);
void kdInsert(KdTree* kdTree, Point point);
Point kdNearestNeighbor(KdTree* kdTree, Point point);
size_t kdGetDepth(KdTree* kdTree);
void kdPrint(KdTree* kdTree);
size_t kdGetDepthNode(Node* node);
double calcSquaredDistance(Point point1, Point point2);
size_t partition(Point *points, size_t length, size_t dimension);

#endif 
