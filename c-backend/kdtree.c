#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "kdtree.h"

Driver drivers[MAX_DRIVERS];
int driverCount = 0;

// Internal structure for kNN
typedef struct {
    int idx;
    double distSq;
} NeighborResult;

// ---------- KD-Tree Core ----------

Node* newNode(double x, double y, int idx, int axis) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->point[0] = x;
    n->point[1] = y;
    n->index = idx;
    n->axis = axis;
    n->left = n->right = NULL;
    return n;
}

Node* insert(Node* root, double x, double y, int idx, int depth) {
    int cd = depth % K;
    if (!root) return newNode(x, y, idx, cd);

    if ((cd == 0 && x < root->point[0]) ||
        (cd == 1 && y < root->point[1]))
        root->left = insert(root->left, x, y, idx, depth + 1);
    else
        root->right = insert(root->right, x, y, idx, depth + 1);

    return root;
}

void nearest(Node* root, double px, double py, int depth, int* bestIdx, double* bestDistSq) {
    if (!root) return;

    double dx = root->point[0] - px;
    double dy = root->point[1] - py;
    double distSq = dx * dx + dy * dy;

    if (distSq < *bestDistSq) {
        *bestDistSq = distSq;
        *bestIdx = root->index;
    }

    int cd = depth % K;
    double val = (cd == 0) ? px : py;
    double split = root->point[cd];

    Node* first = (val < split) ? root->left : root->right;
    Node* second = (val < split) ? root->right : root->left;

    nearest(first, px, py, depth + 1, bestIdx, bestDistSq);

    double diff = val - split;
    if (diff * diff < *bestDistSq)
        nearest(second, px, py, depth + 1, bestIdx, bestDistSq);
}

// k-Nearest Neighbors
void kNearest(Node* root, double px, double py, int depth, 
              NeighborResult* results, int k, int* foundCount) {
    if (!root) return;

    double dx = root->point[0] - px;
    double dy = root->point[1] - py;
    double distSq = dx * dx + dy * dy;

    // Maintain a sorted list of nearest neighbors (simplified max-heap behavior)
    int pos = -1;
    if (*foundCount < k) {
        pos = (*foundCount)++;
    } else if (distSq < results[k-1].distSq) {
        pos = k - 1;
    }

    if (pos != -1) {
        results[pos].idx = root->index;
        results[pos].distSq = distSq;
        // Bubble up to keep sorted by distance
        for (int i = pos; i > 0 && results[i].distSq < results[i-1].distSq; i--) {
            NeighborResult temp = results[i];
            results[i] = results[i-1];
            results[i-1] = temp;
        }
    }

    int cd = depth % K;
    double val = (cd == 0) ? px : py;
    double split = root->point[cd];

    Node* first = (val < split) ? root->left : root->right;
    Node* second = (val < split) ? root->right : root->left;

    kNearest(first, px, py, depth + 1, results, k, foundCount);

    double diff = val - split;
    double maxDistSq = (*foundCount < k) ? DBL_MAX : results[k-1].distSq;
    if (diff * diff < maxDistSq)
        kNearest(second, px, py, depth + 1, results, k, foundCount);
}

// Range Search
void rangeSearch(Node* root, double px, double py, double radiusSq, 
                 int* results, int* resultCount) {
    if (!root) return;

    double dx = root->point[0] - px;
    double dy = root->point[1] - py;
    double distSq = dx * dx + dy * dy;

    if (distSq <= radiusSq) {
        results[(*resultCount)++] = root->index;
    }

    int cd = root->axis;
    double val = (cd == 0) ? px : py;
    double split = root->point[cd];
    double radius = sqrt(radiusSq);

    if (val - radius <= split)
        rangeSearch(root->left, px, py, radiusSq, results, resultCount);
    if (val + radius >= split)
        rangeSearch(root->right, px, py, radiusSq, results, resultCount);
}

void freeTree(Node* root) {
    if (!root) return;
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
    int idx = driverCount;
    driverCount++;
    return idx;
}

int removeDriver(int index) {
    if (index < 0 || index >= driverCount) return 0;
    if (!drivers[index].active) return 0;

    drivers[index].active = 0;
    return 1;
}

int updateDriverPosition(int index, double newX, double newY) {
    if (index < 0 || index >= driverCount || !drivers[index].active)
        return 0;
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
        *outDist = 0;
        return 0;
    }

    double bestDistSq = DBL_MAX;
    int bestIdx = -1;
    nearest(root, px, py, 0, &bestIdx, &bestDistSq);

    *outIdx = bestIdx;
    *outDist = sqrt(bestDistSq);

    freeTree(root);
    return 1;
}

int findKNearestDrivers(double px, double py, int k, int* outIndices, double* outDists) {
    Node* root = NULL;
    int activeCount = 0;
    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].active) {
            root = insert(root, drivers[i].x, drivers[i].y, i, 0);
            activeCount++;
        }
    }

    if (activeCount == 0 || k <= 0) return 0;
    if (k > activeCount) k = activeCount;

    NeighborResult* results = (NeighborResult*)malloc(k * sizeof(NeighborResult));
    for (int i = 0; i < k; i++) {
        results[i].distSq = DBL_MAX;
        results[i].idx = -1;
    }
    int foundCount = 0;

    kNearest(root, px, py, 0, results, k, &foundCount);

    for (int i = 0; i < foundCount; i++) {
        outIndices[i] = results[i].idx;
        outDists[i] = sqrt(results[i].distSq);
    }

    int finalCount = foundCount;
    free(results);
    freeTree(root);
    return finalCount;
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

    int* results = (int*)malloc(activeCount * sizeof(int));
    int resultCount = 0;
    rangeSearch(root, px, py, radius * radius, results, &resultCount);

    for (int i = 0; i < resultCount; i++) {
        outIndices[i] = results[i];
    }

    int finalCount = resultCount;
    free(results);
    freeTree(root);
    return finalCount;
}

int getDriverCount(void) {
    int count = 0;
    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].active) count++;
    }
    return count;
}