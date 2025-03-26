#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "kd_tree.h"
#include "array_list.h"


size_t partition(Point *points, size_t length, size_t dimension) {
    double pivot = getDimension(&points[length - 1], dimension);
    size_t i = 0;

    for (size_t j = i; j < length - 1; j++) {
        if (getDimension(&points[j], dimension) <= pivot) {
            Point temp = points[i];
            points[i] = points[j];
            points[j] = temp;
            i++;
        }
    }
    Point temp = points[i];
    points[i] = points[length - 1];
    points[length - 1] = temp;
    return i;
}

size_t quickselect(Point *points, size_t left, size_t right, size_t k, size_t dimension) {
    while (true) {
        if (left == right) {
            return left;
        }
        size_t pivotIndex = left + partition(points + left, right - left + 1, dimension);
        if (pivotIndex == k) {
            return k;
        } else if (k < pivotIndex) {
            right =  pivotIndex - 1;
        } else {
            left = pivotIndex + 1;
        }
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

    size_t pivot_index = quickselect(points, 0, pointsLength - 1, pointsLength / 2, dimension);

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
