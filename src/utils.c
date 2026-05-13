#include <stdlib.h>
#include <string.h>
#include "process.h"

// Swap helper
void swap_process(Process *a, Process *b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

// Sort by arrival time (FCFS base)
void sort_by_arrival(Process *p, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                swap_process(&p[j], &p[j + 1]);
            }
        }
    }
}

// Sort by burst time (SJF helper)
void sort_by_burst(Process *p, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].burst_time > p[j + 1].burst_time) {
                swap_process(&p[j], &p[j + 1]);
            }
        }
    }
}