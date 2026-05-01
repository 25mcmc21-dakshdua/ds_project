#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "kdtree.h"

Driver drivers[MAX_DRIVERS];
int driverCount = 0;

// Create Node
Node* newNode(int coords[K], int idx) {
    Node* n = (Node*)malloc(sizeof(Node));
    for (int i = 0; i < K; i++) {
        n->point[i] = coords[i];
    }
    n->index = idx;
    n->left = n->right = NULL;
    return n;
}

// Insert
Node* insert(Node* root, int coords[K], int idx, int depth) {
    if (root == NULL) {
        return newNode(coords, idx);
    }

    int axis = depth % K;

    if (coords[axis] < root->point[axis]) {
        root->left = insert(root->left, coords, idx, depth + 1);
    } else {
        root->right = insert(root->right, coords, idx, depth + 1);
    }

    return root;
}

// Distance function
double distanceSquared(int p1[K], int p2[K]) {
    double sum = 0;
    for (int i = 0; i < K; i++) {
        double diff = (double)p1[i] - (double)p2[i];
        sum += diff * diff;
    }
    return sum;
}

// Nearest Neighbor
void nearest(Node* root, int target[K], int* bestIdx, double* bestDistSq, int depth) {
    if (root == NULL)
        return;

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

    nearest(nextBranch, target, bestIdx, bestDistSq, depth + 1);

    double diff = (double)target[axis] - (double)root->point[axis];

    if (diff * diff < *bestDistSq) {
        nearest(otherBranch, target, bestIdx, bestDistSq, depth + 1);
    }
}

// Range Search
void radiusSearch(Node* root, int target[K], double radius, int depth, int* results, int* count) {
    if (root == NULL)
        return;

    double dSq = distanceSquared(root->point, target);

    if (dSq <= radius * radius) {
        results[(*count)++] = root->index;
    }

    int axis = depth % K;

    if ((double)target[axis] - radius <= (double)root->point[axis]) {
        radiusSearch(root->left, target, radius, depth + 1, results, count);
    }
    if ((double)target[axis] + radius >= (double)root->point[axis]) {
        radiusSearch(root->right, target, radius, depth + 1, results, count);
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

int findNearestDriver(double px, double py, int* outIdx, double* outDist) {
    Node* root = NULL;
    int activeCount = 0;
    int target[K] = {(int)px, (int)py};

    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].active) {
            int coords[K] = {(int)drivers[i].x, (int)drivers[i].y};
            root = insert(root, coords, i, 0);
            activeCount++;
        }
    }

    if (activeCount == 0) {
        *outIdx = -1;
        return 0;
    }

    double bestDistSq = DBL_MAX;
    int bestIdx = -1;
    nearest(root, target, &bestIdx, &bestDistSq, 0);
    *outIdx = bestIdx;
    *outDist = sqrt(bestDistSq);
    freeTree(root);
    return 1;
}

int findDriversInRadius(double px, double py, double radius, int* outIndices) {
    Node* root = NULL;
    int activeCount = 0;
    int target[K] = {(int)px, (int)py};

    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].active) {
            int coords[K] = {(int)drivers[i].x, (int)drivers[i].y};
            root = insert(root, coords, i, 0);
            activeCount++;
        }
    }

    if (activeCount == 0) return 0;

    int count = 0;
    radiusSearch(root, target, radius, 0, outIndices, &count);
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