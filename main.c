#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define REQUEST_COUNT 10

// Function prototype for scheduling algorithms
int fcfs_schedule(int start, int requests[], int count, int result[]);
int sstf_schedule(int start, int requests[], int count, int result[]);

int main() {
    int start = 2255;
    int requests[REQUEST_COUNT] = {2055, 1175, 2304, 2700, 513, 1680, 256, 1401, 4922, 3692};
    int order_fcfs[REQUEST_COUNT];
    int order_sstf[REQUEST_COUNT];

    // Run FCFS
    int total_fcfs = fcfs_schedule(start, requests, REQUEST_COUNT, order_fcfs);
    printf("=== FCFS ===\nOrder of Service with Movements:\n");
    int current = start;
    for (int i = 0; i < REQUEST_COUNT; i++) {
        int movement = abs(current - order_fcfs[i]);
        printf("From %d to %d: moved %d cylinders\n", current, order_fcfs[i], movement);
        current = order_fcfs[i];
    }
    printf("Total Distance Moved: %d cylinders\n\n", total_fcfs);

    // Run SSTF
    int total_sstf = sstf_schedule(start, requests, REQUEST_COUNT, order_sstf);
    printf("=== SSTF ===\nOrder of Service with Movements:\n");
    current = start;
    for (int i = 0; i < REQUEST_COUNT; i++) {
        int movement = abs(current - order_sstf[i]);
        printf("From %d to %d: moved %d cylinders\n", current, order_sstf[i], movement);
        current = order_sstf[i];
    }
    printf("Total Distance Moved: %d cylinders\n", total_sstf);

    return 0;
}


int fcfs_schedule(int start, int requests[], int count, int result[]) {
    int total = 0;
    int current = start;

    for (int i = 0; i < count; i++) {
        result[i] = requests[i];
        total += abs(current - requests[i]);
        current = requests[i];
    }

    return total;
}

int sstf_schedule(int start, int requests[], int count, int result[]) {
    int total = 0;
    int current = start;
    int processed[REQUEST_COUNT] = {0}; // 0 = not processed, 1 = processed

    for (int i = 0; i < count; i++) {
        int min_distance = __INT_MAX__;
        int index = -1;

        for (int j = 0; j < count; j++) {
            if (!processed[j]) {
                int distance = abs(current - requests[j]);
                if (distance < min_distance) {
                    min_distance = distance;
                    index = j;
                }
            }
        }
        if (index != -1) {
            result[i] = requests[index];
            total += abs(current - requests[index]);
            current = requests[index];
            processed[index] = 1;
        }
    }
    return total;
}
