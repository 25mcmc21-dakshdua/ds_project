#ifndef KDTREE_H
#define KDTREE_H

#define MAX_DRIVERS 100
#define K 2

typedef struct {
    double x, y;
    int active; // 1 = active, 0 = removed
} Driver;

typedef struct Node {
    int point[K];
    int index;
    struct Node *left, *right;
} Node;

// KD-tree operations
Node* newNode(int coords[K], int idx);
Node* insert(Node* root, int coords[K], int idx, int depth);
void nearest(Node* root, int target[K], int* bestIdx, double* bestDistSq, int depth);
void freeTree(Node* root);

// Driver management
int addDriver(double x, double y);
int removeDriver(int index);
int findNearestDriver(double px, double py, int* outIdx, double* outDist);
int findDriversInRadius(double px, double py, double radius, int* outIndices);
int getDriverCount(void);

// Global state
extern Driver drivers[MAX_DRIVERS];
extern int driverCount;

#endif
