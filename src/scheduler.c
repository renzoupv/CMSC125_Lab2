#include <stdio.h>
#include <string.h>
#include "scheduler.h"

// algorithm headers
int run_fcfs(Process *p, int n);
int run_sjf(Process *p, int n);
int run_stcf(Process *p, int n);
int run_rr(Process *p, int n, int quantum);
int run_mlfq(Process *p, int n);

int schedule(Process *processes, int n, char *algorithm, int quantum) {

    if (strcmp(algorithm, "FCFS") == 0) {
        return run_fcfs(processes, n);
    }

    if (strcmp(algorithm, "SJF") == 0) {
        return run_sjf(processes, n);
    }

    if (strcmp(algorithm, "STCF") == 0) {
        return run_stcf(processes, n);
    }

    if (strcmp(algorithm, "RR") == 0) {
        return run_rr(processes, n, quantum);
    }

    if (strcmp(algorithm, "MLFQ") == 0) {
        return run_mlfq(processes, n);
    }

    printf("Unknown algorithm: %s\n", algorithm);
    return -1;
}