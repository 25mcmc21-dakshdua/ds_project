#ifndef KDTREE_H
#define KDTREE_H

#define MAX_DRIVERS 100
#define K 2

typedef struct {
    double x, y;
    int active; // 1 = active, 0 = removed
} Driver;

typedef struct Node {
    double point[K];
    int index;
    int axis; // Added for range search
    struct Node *left, *right;
} Node;

// KD-tree operations
Node* newNode(double x, double y, int idx, int axis);
Node* insert(Node* root, double x, double y, int idx, int depth);
void nearest(Node* root, double px, double py, int* bestIdx, double* bestDist, int depth);
void freeTree(Node* root);

// Driver management
int addDriver(double x, double y);
int removeDriver(int index);
int updateDriverPosition(int index, double newX, double newY);
int findNearestDriver(double px, double py, int* outIdx, double* outDist);
int findDriversInRadius(double px, double py, double radius, int* outIndices);
int getDriverCount(void);

// Global state
extern Driver drivers[MAX_DRIVERS];
extern int driverCount;

#endif
