#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "kd_tree.h"
#include "array_list.h"


double getDimension(Point* point, size_t dimension) {
    if (dimension == 0) {
        return point->x;
    }
    return point->y;
}

size_t partition(Point *points, size_t length, size_t dimension) {
    Point pivot = points[0];
    int left = -1;
    int right = length;
    while (1) {
        do {
            left++;
        } while (getDimension(&points[left], dimension) < getDimension(&pivot, dimension));
        do {
            right--;
        } while (getDimension(&points[right], dimension) > getDimension(&pivot, dimension));

        if (left >= right) {
            bool foundPivot = false;
            for (size_t i = 0; i < length; i++) {
                if (getDimension(&points[i], dimension) == getDimension(&pivot, dimension)) {
                    foundPivot = true;
                    continue;
                }
                if (foundPivot && getDimension(&points[i], dimension) < getDimension(&pivot, dimension)) {
                    printf("Error\n");
                    printf("Index %zu not partitioned correctly.\n", i);
                    printf("pivot: %.2f\n", getDimension(&pivot, dimension));
                    printf("partitioned points:\n");
                    for (size_t i = 0; i < length; i++) {
                        printf("%.2f, ", getDimension(&points[i], dimension));
                    }
                    printf("\n");
                    exit(1);
                }
                if (!foundPivot && getDimension(&points[i], dimension) > getDimension(&pivot, dimension)) {
                    printf("Error\n");
                    printf("Index %zu not partitioned correctly.\n", i);
                    printf("pivot: %.2f\n", getDimension(&pivot, dimension));
                    printf("partitioned points:\n");
                    for (size_t i = 0; i < length; i++) {
                        printf("%.2f, ", getDimension(&points[i], dimension));
                    }
                    printf("\n");
                    exit(1);
                }
            }
            return right;
            if (right - 1 >= 0 && getDimension(&points[right - 1], dimension) == getDimension(&pivot, dimension)) {
                return right - 1;
            }
            else if (getDimension(&points[right], dimension) == getDimension(&pivot, dimension)) {
                return right;
            }
            else if ((size_t)(right + 1) < length && getDimension(&points[right + 1], dimension) == getDimension(&pivot, dimension)) {
                return right + 1;
            } else {
                printf("Error\n");
                printf("pivot: %.2f\n", getDimension(&pivot, dimension));
                printf("pivot_index %d\n", right);
                printf("partitioned points:\n");
                for (size_t i = 0; i < length; i++) {
                    printf("%.2f, ", getDimension(&points[i], dimension));
                }
                printf("\n");
                exit(1);
            }
        }
        Point temp = points[left];
        points[left] = points[right];
        points[right] = temp;
    }
}

size_t globalDimension = 0;


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

Node* kdInitNodeMedian(Point* points, size_t pointsLength, size_t dimension) {
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
    node->leftChild = kdInitNodeMedian(points, pointsLength / 2, newDimension);
    node->rightChild = kdInitNodeMedian(points + pointsLength / 2 + 1, pointsLength / 2 - (1 - pointsLength % 2), newDimension);
    return node;
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

    size_t pivot_index = partition(points, pointsLength, dimension);

    size_t newDimension = (dimension + 1) % 2;
    node->point = points[pivot_index];
    node->leftChild = kdInitNode(points, pivot_index, newDimension);
    node->rightChild = kdInitNode(points + pivot_index + 1, pointsLength - pivot_index - 1, newDimension);
    return node;
}

KdTree* kdInitEmpty() {
    KdTree* kdTree = (KdTree*)malloc(sizeof(KdTree));
    kdTree->root = NULL;
    return kdTree;
}

KdTree* kdInit(Point* points, size_t pointsLength) {
    KdTree* kdTree = (KdTree*)malloc(sizeof(KdTree));
    kdTree->root = kdInitNode(points, pointsLength, 0);
    return kdTree;
}

KdTree* kdInitBalanced(Point* points, size_t pointsLength) {
    KdTree* kdTree = (KdTree*)malloc(sizeof(KdTree));
    kdTree->root = kdInitNodeMedian(points, pointsLength, 0);
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

void kdInsertNode(Node *node, Node *nodeToInsert, size_t dimension) {
    size_t newDimension = (dimension + 1) % 2;
    if  (getDimension(&nodeToInsert->point, dimension) < getDimension(&node->point, dimension)) {
        if (node->leftChild == NULL) {
            node->leftChild = nodeToInsert;
        } else {
            kdInsertNode(node->leftChild, nodeToInsert, newDimension);
        }
    } else {
        if (node->rightChild == NULL) {
            node->rightChild = nodeToInsert;
        } else {
            kdInsertNode(node->rightChild, nodeToInsert, newDimension);
        }
    }
}

void kdInsert(KdTree* kdTree, Point point) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->point = point;
    node->leftChild = NULL;
    node->rightChild = NULL;
    if (kdTree->root == NULL) {
        kdTree->root = node;
        return;
    }
    kdInsertNode(kdTree->root, node, 0);
}

size_t kdGetDepthNode(Node* node) {
    if (node == NULL) {
        return 0;
    }
    size_t leftDepth = kdGetDepthNode(node->leftChild);
    size_t rightDepth = kdGetDepthNode(node->rightChild);
    if (rightDepth > leftDepth) {
        return rightDepth + 1;
    }
    return leftDepth + 1;
}

size_t kdGetDepth(KdTree* kdTree) {
    return kdGetDepthNode(kdTree->root);
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
