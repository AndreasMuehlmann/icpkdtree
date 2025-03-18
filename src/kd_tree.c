#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "kd_tree.h"
#include "array_list.h"


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

double getX(Point point) {
    return point.x;
}

double getY(Point point) {
    return point.y;
}

ssize_t binarySearch(Point *points, size_t pointsLength, double searched, double (*getCoordinate)(Point)) {
    size_t low = 0;
    size_t high = pointsLength - 1;

    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (getCoordinate(points[mid]) < searched) {
            low = mid + 1;
        } else if (getCoordinate(points[mid]) > searched) {
            high = mid - 1;
        } else {
            return mid;
        }
    }
    if (getCoordinate(points[low]) == searched) {
        return low;
    }
    return -1;
}

size_t max(ssize_t value1, ssize_t value2) {
    if (value1 > value2) {
        return value1;
    } else {
        return value2;
    }
}

Node* kdInitNode(Point* sortedX, size_t sortedXLength, Point* sortedY, size_t sortedYLength, bool isDimensionX) {
    if (isDimensionX) {
        if (sortedXLength == 0) {
            return NULL;
        }
        Node* node = (Node*)malloc(sizeof(Node));
        if (sortedXLength == 1) {
            node->point = sortedX[0];
            node->leftChild = NULL;
            node->rightChild = NULL;
            return node;
        }
        node->point = sortedX[sortedXLength / 2];


        ssize_t pivot = binarySearch(sortedY, sortedYLength, node->point.y, getY);
        if (pivot == -1) {
            printf("Couldn't find y coordinate\n");
            exit(1);
        }

        node->leftChild = kdInitNode(sortedX, sortedXLength / 2, sortedY, pivot, isDimensionX); // add the not to isDimensionX
        node->rightChild = kdInitNode(sortedX + sortedXLength / 2 + 1, sortedXLength / 2 - 1, sortedY + pivot + 1, max(0, sortedYLength - pivot - 1), isDimensionX);
        return node;
    }
    return NULL;
}

KdTree* kdInit(Point* points, size_t pointsLength) {
    KdTree* kdTree = (KdTree*)malloc(sizeof(KdTree));
    qsort(points, pointsLength, sizeof(Point), compareX);

    Point* pointsSortedY = (Point*)malloc(sizeof(Point) * pointsLength);
    memcpy(pointsSortedY, points, sizeof(Point) * pointsLength);

    qsort(pointsSortedY, pointsLength, sizeof(Point), compareY);

    kdTree->root = kdInitNode(points, pointsLength, pointsSortedY, pointsLength, true);

    free(pointsSortedY);
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
