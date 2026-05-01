#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdtree.h"

/*
 * KD-Tree IPC Server
 * Reads commands from stdin, writes JSON responses to stdout.
 *
 * Protocol:
 *   ADD|x|y       -> {"status":"ok","index":N,"x":X,"y":Y}
 *   FIND|x|y      -> {"status":"ok","index":N,"x":X,"y":Y,"distance":D}
 *   REMOVE|index   -> {"status":"ok","removed":1}
 *   LIST           -> {"status":"ok","count":N,"drivers":[...]}
 *   CLEAR          -> {"status":"ok"}
 *   EXIT           -> (terminates)
 */

void handleAdd(char* args) {
    double x, y;
    if (sscanf(args, "%lf|%lf", &x, &y) != 2) {
        printf("{\"status\":\"error\",\"message\":\"Invalid ADD args\"}\n");
        fflush(stdout);
        return;
    }

    int idx = addDriver(x, y);
    if (idx < 0) {
        printf("{\"status\":\"error\",\"message\":\"Max drivers reached\"}\n");
    } else {
        printf("{\"status\":\"ok\",\"index\":%d,\"x\":%.2f,\"y\":%.2f}\n", idx, x, y);
    }
    fflush(stdout);
}

void handleFind(char* args) {
    double x, y;
    if (sscanf(args, "%lf|%lf", &x, &y) != 2) {
        printf("{\"status\":\"error\",\"message\":\"Invalid FIND args\"}\n");
        fflush(stdout);
        return;
    }

    int foundIdx;
    double dist;
    if (findNearestDriver(x, y, &foundIdx, &dist)) {
        printf("{\"status\":\"ok\",\"index\":%d,\"x\":%.2f,\"y\":%.2f,\"distance\":%.2f}\n",
               foundIdx, drivers[foundIdx].x, drivers[foundIdx].y, dist);
    } else {
        printf("{\"status\":\"error\",\"message\":\"No drivers available\"}\n");
    }
    fflush(stdout);
}

void handleRemove(char* args) {
    int index;
    if (sscanf(args, "%d", &index) != 1) {
        printf("{\"status\":\"error\",\"message\":\"Invalid REMOVE args\"}\n");
        fflush(stdout);
        return;
    }

    int result = removeDriver(index);
    printf("{\"status\":\"ok\",\"removed\":%d}\n", result);
    fflush(stdout);
}

void handleList() {
    int activeCount = getDriverCount();
    printf("{\"status\":\"ok\",\"count\":%d,\"drivers\":[", activeCount);

    int first = 1;
    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].active) {
            if (!first) printf(",");
            printf("{\"index\":%d,\"x\":%.2f,\"y\":%.2f}", i, drivers[i].x, drivers[i].y);
            first = 0;
        }
    }

    printf("]}\n");
    fflush(stdout);
}



void handleRange(char* args) {
    double x, y, radius;
    if (sscanf(args, "%lf|%lf|%lf", &x, &y, &radius) != 3) {
        printf("{\"status\":\"error\",\"message\":\"Invalid RANGE args\"}\n");
        fflush(stdout);
        return;
    }

    int indices[MAX_DRIVERS];
    int count = findDriversInRadius(x, y, radius, indices);

    printf("{\"status\":\"ok\",\"count\":%d,\"results\":[", count);
    for (int i = 0; i < count; i++) {
        int idx = indices[i];
        if (i > 0) printf(",");
        printf("{\"index\":%d,\"x\":%.2f,\"y\":%.2f}",
               idx, drivers[idx].x, drivers[idx].y);
    }
    printf("]}\n");
    fflush(stdout);
}

void handleClear() {
    for (int i = 0; i < driverCount; i++) {
        drivers[i].active = 0;
    }
    driverCount = 0;
    printf("{\"status\":\"ok\"}\n");
    fflush(stdout);
}

int main() {
    char line[256];

    // Signal ready
    printf("{\"status\":\"ready\"}\n");
    fflush(stdout);

    while (fgets(line, sizeof(line), stdin)) {
        // Strip newline
        line[strcspn(line, "\r\n")] = 0;

        if (strlen(line) == 0) continue;

        // Parse command
        char* cmd = strtok(line, "|");
        char* args = cmd + strlen(cmd) + 1;

        if (strcmp(cmd, "ADD") == 0) {
            handleAdd(args);
        } else if (strcmp(cmd, "FIND") == 0) {
            handleFind(args);
        } else if (strcmp(cmd, "REMOVE") == 0) {
            handleRemove(args);
        } else if (strcmp(cmd, "LIST") == 0) {
            handleList();
        } else if (strcmp(cmd, "RANGE") == 0) {
            handleRange(args);
        } else if (strcmp(cmd, "CLEAR") == 0) {
            handleClear();
        } else if (strcmp(cmd, "EXIT") == 0) {
            break;
        } else {
            printf("{\"status\":\"error\",\"message\":\"Unknown command\"}\n");
            fflush(stdout);
        }
    }

    return 0;
}
