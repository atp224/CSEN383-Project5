#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define REQUEST_COUNT 10
#define CYLINDER_MAX 4999   // Assuming max cylinder number (end of disk)

int fcfs_schedule(int start, int requests[], int count, int result[]);
int sstf_schedule(int start, int requests[], int count, int result[]);
int scan_schedule(int start, int requests[], int count, int result[], int direction); // direction: 0=left, 1=right

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main() {
    int start = 2255;
    int requests[REQUEST_COUNT] = {2055, 1175, 2304, 2700, 513, 1680, 256, 1401, 4922, 3692};
    int order_fcfs[REQUEST_COUNT];
    int order_sstf[REQUEST_COUNT];
    int order_scan[REQUEST_COUNT];

    // FCFS
    int total_fcfs = fcfs_schedule(start, requests, REQUEST_COUNT, order_fcfs);
    printf("=== FCFS ===\nOrder of Service:\n");
    for (int i = 0; i < REQUEST_COUNT; i++) {
        printf("%d ", order_fcfs[i]);
    }
    printf("\nTotal Distance Moved: %d cylinders\n\n", total_fcfs);

    // SSTF
    int total_sstf = sstf_schedule(start, requests, REQUEST_COUNT, order_sstf);
    printf("=== SSTF ===\nOrder of Service:\n");
    for (int i = 0; i < REQUEST_COUNT; i++) {
        printf("%d ", order_sstf[i]);
    }
    printf("\nTotal Distance Moved: %d cylinders\n\n", total_sstf);

    // SCAN (Assuming initial direction is RIGHT)
    int total_scan = scan_schedule(start, requests, REQUEST_COUNT, order_scan, 1);
    printf("=== SCAN (Right) ===\nOrder of Service:\n");
    for (int i = 0; i < REQUEST_COUNT; i++) {
        printf("%d ", order_scan[i]);
    }
    printf("\nTotal Distance Moved: %d cylinders\n", total_scan);

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
    int processed[REQUEST_COUNT] = {0};

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

int scan_schedule(int start, int requests[], int count, int result[], int direction) {
    int total = 0;
    int current = start;
    int sorted[REQUEST_COUNT];
    for (int i = 0; i < count; i++) {
        sorted[i] = requests[i];
    }

    qsort(sorted, count, sizeof(int), compare);

    int index = 0;
    while (index < count && sorted[index] < current) {
        index++;
    }

    int res_index = 0;

    if (direction == 1) { // Moving right
        for (int i = index; i < count; i++) {
            result[res_index++] = sorted[i];
            total += abs(current - sorted[i]);
            current = sorted[i];
        }
        if (index > 0) {
            total += abs(current - CYLINDER_MAX);
            current = CYLINDER_MAX;
            for (int i = index - 1; i >= 0; i--) {
                result[res_index++] = sorted[i];
                total += abs(current - sorted[i]);
                current = sorted[i];
            }
        }
    } else { // Moving left
        for (int i = index - 1; i >= 0; i--) {
            result[res_index++] = sorted[i];
            total += abs(current - sorted[i]);
            current = sorted[i];
        }
        if (index < count) {
            total += abs(current - 0);
            current = 0;
            for (int i = index; i < count; i++) {
                result[res_index++] = sorted[i];
                total += abs(current - sorted[i]);
                current = sorted[i];
            }
        }
    }

    return total;
}
