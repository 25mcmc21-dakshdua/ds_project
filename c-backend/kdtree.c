#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "kdtree.h"

Driver drivers[MAX_DRIVERS];
int driverCount = 0;

// Create Node
Node* newNode(double x, double y, int idx, int axis) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->point[0] = x;
    n->point[1] = y;
    n->index = idx;
    n->axis = axis;
    n->left = n->right = NULL;
    return n;
}

// Insert
Node* insert(Node* root, double x, double y, int idx, int depth) {
    if (root == NULL) {
        return newNode(x, y, idx, depth % K);
    }

    int axis = depth % K;

    if (axis == 0) { // X-axis
        if (x < root->point[0])
            root->left = insert(root->left, x, y, idx, depth + 1);
        else
            root->right = insert(root->right, x, y, idx, depth + 1);
    } else { // Y-axis
        if (y < root->point[1])
            root->left = insert(root->left, x, y, idx, depth + 1);
        else
            root->right = insert(root->right, x, y, idx, depth + 1);
    }

    return root;
}

// Distance function
double distanceSquared(double p1[2], double p2[2]) {
    double dx = p1[0] - p2[0];
    double dy = p1[1] - p2[1];
    return dx * dx + dy * dy;
}

// Nearest Neighbor
void nearest(Node* root, double targetX, double targetY, 
             int* bestIdx, double* bestDistSq, int depth) {
    if (root == NULL)
        return;

    double target[2] = {targetX, targetY};
    double d = distanceSquared(root->point, target);

    if (d < *bestDistSq) {
        *bestDistSq = d;
        *bestIdx = root->index;
    }

    int axis = depth % K;

    Node *nextBranch, *otherBranch;

    if (target[axis] < root->point[axis]) {
        nextBranch = root->left;
        otherBranch = root->right;
    } else {
        nextBranch = root->right;
        otherBranch = root->left;
    }

    nearest(nextBranch, targetX, targetY, bestIdx, bestDistSq, depth + 1);

    double diff = target[axis] - root->point[axis];

    if (diff * diff < *bestDistSq) {
        nearest(otherBranch, targetX, targetY, bestIdx, bestDistSq, depth + 1);
    }
}

// Range Search
void radiusSearch(Node* root, double targetX, double targetY, double radius, int depth, int* results, int* count) {
    if (root == NULL)
        return;

    double dx = root->point[0] - targetX;
    double dy = root->point[1] - targetY;
    double distSq = dx * dx + dy * dy;

    if (distSq <= radius * radius) {
        results[(*count)++] = root->index;
    }

    int axis = depth % K;
    double targetVal = (axis == 0) ? targetX : targetY;

    if (targetVal - radius <= root->point[axis]) {
        radiusSearch(root->left, targetX, targetY, radius, depth + 1, results, count);
    }
    if (targetVal + radius >= root->point[axis]) {
        radiusSearch(root->right, targetX, targetY, radius, depth + 1, results, count);
    }
}

// Free tree
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// ---------- Driver Management ----------

int addDriver(double x, double y) {
    if (driverCount >= MAX_DRIVERS) return -1;
    drivers[driverCount].x = x;
    drivers[driverCount].y = y;
    drivers[driverCount].active = 1;
    return driverCount++;
}

int removeDriver(int index) {
    if (index < 0 || index >= driverCount || !drivers[index].active) return 0;
    drivers[index].active = 0;
    return 1;
}

int updateDriverPosition(int index, double newX, double newY) {
    if (index < 0 || index >= driverCount || !drivers[index].active) return 0;
    drivers[index].x = newX;
    drivers[index].y = newY;
    return 1;
}

int findNearestDriver(double px, double py, int* outIdx, double* outDist) {
    Node* root = NULL;
    int activeCount = 0;
    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].active) {
            root = insert(root, drivers[i].x, drivers[i].y, i, 0);
            activeCount++;
        }
    }
    if (activeCount == 0) {
        *outIdx = -1;
        return 0;
    }
    double bestDistSq = DBL_MAX;
    int bestIdx = -1;
    nearest(root, px, py, &bestIdx, &bestDistSq, 0);
    *outIdx = bestIdx;
    *outDist = sqrt(bestDistSq);
    freeTree(root);
    return 1;
}

int findDriversInRadius(double px, double py, double radius, int* outIndices) {
    Node* root = NULL;
    int activeCount = 0;
    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].active) {
            root = insert(root, drivers[i].x, drivers[i].y, i, 0);
            activeCount++;
        }
    }
    if (activeCount == 0) return 0;

    int count = 0;
    radiusSearch(root, px, py, radius, 0, outIndices, &count);
    freeTree(root);
    return count;
}

int getDriverCount(void) {
    int count = 0;
    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].active) count++;
    }
    return count;
}