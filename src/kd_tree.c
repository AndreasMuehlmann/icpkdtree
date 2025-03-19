#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "kd_tree.h"
#include "array_list.h"


size_t globalDimension = 0;


double getDimension(Point* point, size_t dimension) {
    if (dimension == 0) {
        return point->x;
    }
    return point->y;
}


int compare(const void* a, const void* b) {
    double value1 = getDimension((Point*)a, globalDimension);
    double value2 = getDimension((Point*)b, globalDimension);
    if (value1 > value2) {
        return 1;
    } else if (value1 < value2) {
        return -1;
    } else {
        return 0;
    }
}

Node* kdInitNode(Point* points, size_t pointsLength, size_t dimension) {
    if (pointsLength == 0) {
        return NULL;
    }
    Node* node = (Node*)malloc(sizeof(Node));
    if (pointsLength == 1) {
        node->point = points[0];
        node->leftChild = NULL;
        node->rightChild = NULL;
        return node;
    }

    globalDimension = dimension;
    qsort(points, pointsLength, sizeof(Point), compare);

    size_t newDimension = (dimension + 1) % 2;
    node->point = points[pointsLength / 2];
    node->leftChild = kdInitNode(points, pointsLength / 2, newDimension);
    node->rightChild = kdInitNode(points + pointsLength / 2 + 1, pointsLength / 2 - (1 - pointsLength % 2), newDimension);
    return node;
}

KdTree* kdInit(Point* points, size_t pointsLength) {
    KdTree* kdTree = (KdTree*)malloc(sizeof(KdTree));
    kdTree->root = kdInitNode(points, pointsLength, 0);
    return kdTree;
}

void kdDeinitNode(Node* node) {
    if (node == NULL) {
        return;
    }
    kdDeinitNode(node->leftChild);
    kdDeinitNode(node->rightChild);
    free(node);
}

void kdDeinit(KdTree* kdTree) {
    if (kdTree->root != NULL) {
        kdDeinitNode(kdTree->root);
    }
    free(kdTree);
}

typedef struct {
    Node* node;
    size_t layer;
} NodeWithLayer;

void kdPrint(KdTree* kdTree) {
    ArrayList* queue = arrayListInit(sizeof(NodeWithLayer));
    ArrayList* layers = arrayListInit(sizeof(Point*));


    NodeWithLayer root = {
        kdTree->root,
        0,

    };
    arrayListAppend(queue, &root);
    for (size_t i = 0; i < queue->len; i++) {
        NodeWithLayer node;
        arrayListGet(queue, &node, i);
        if (node.node->leftChild != NULL) {
            NodeWithLayer leftNodeWithLayer = {
                node.node->leftChild,
                node.layer + 1,

            };
            arrayListAppend(queue, &leftNodeWithLayer);
        }
        if (node.node->rightChild != NULL) {
            NodeWithLayer rightNodeWithLayer = {
                node.node->rightChild,
                node.layer + 1,

            };
            arrayListAppend(queue, &rightNodeWithLayer);
        }
    }

    printf("kd tree print:\n");
    size_t current_layer = 0;
    for (size_t i = 0; i < queue->len; i++) {
        NodeWithLayer node;
        arrayListGet(queue, &node, i);
        if (node.layer != current_layer) {
            current_layer = node.layer;
            printf("\n\n");
        }
        printf("%.2f, %.2f; ", node.node->point.x, node.node->point.y);
    }
    printf("\n");
    arrayListDeinit(queue);
    arrayListDeinit(layers);
}

double calcSquaredDistance(Point point1, Point point2) {
    double xDiff = point2.x - point1.x;
    double yDiff = point2.y - point1.x;
    return xDiff * xDiff + yDiff * yDiff;
}

Point kdNearestNeighborNode(Node* node, Point point, size_t dimension) {
    double treeValue = getDimension(&node->point, dimension);
    double value = getDimension(&point, dimension);

    size_t newDimension = (dimension + 1) % 2;

    Point nearestNeighbor;
    Node *otherSubtree;
    if (value < treeValue) {
        if (node->leftChild == NULL) {
            return node->point;
        }
        nearestNeighbor = kdNearestNeighborNode(node->leftChild, point, newDimension);
        otherSubtree = node->rightChild;
    } else {
        if (node->rightChild == NULL) {
            return node->point;
        }
        nearestNeighbor = kdNearestNeighborNode(node->rightChild, point, newDimension);
        otherSubtree = node->leftChild;
    }

    
    if (calcSquaredDistance(point, node->point) < calcSquaredDistance(point, nearestNeighbor)) {
        nearestNeighbor = node->point;     
    }

    if (otherSubtree == NULL) {
        return nearestNeighbor;
    }

    if ((treeValue - value) * (treeValue - value) < calcSquaredDistance(nearestNeighbor, point)) {
        Point nearestNeighborOtherSubtree = kdNearestNeighborNode(otherSubtree, point, newDimension);
        if (calcSquaredDistance(point, nearestNeighborOtherSubtree) < calcSquaredDistance(point, nearestNeighbor)) {
            nearestNeighbor = nearestNeighborOtherSubtree;     
        }
    }

    return nearestNeighbor;
}

Point kdNearestNeighbor(KdTree* kdTree, Point point) {
    return kdNearestNeighborNode(kdTree->root, point, 0);
}
