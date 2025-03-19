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
double calcSquaredDistance(Point point1, Point point2);
Point kdNearestNeighbor(KdTree* kdTree, Point point);
size_t kdGetDepthNode(Node* node);
size_t kdGetDepth(KdTree* kdTree);
KdTree* kdInitEmpty();
void kdInsert(KdTree* kdTree, Point point);

#endif 
