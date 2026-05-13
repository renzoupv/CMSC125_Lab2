#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "metrics.h"

int schedule(Process *processes, int n, char *algorithm, int quantum) {
    SchedulerState s;
    s.processes = processes;
    s.n = n;
    s.time = 0;

    if (strcmp(algorithm, "FCFS") == 0) {
        run_fcfs(&s);
    } else if (strcmp(algorithm, "SJF") == 0) {
        run_sjf(&s);
    } else if (strcmp(algorithm, "STCF") == 0) {
        run_stcf(&s);
    } else if (strcmp(algorithm, "RR") == 0) {
        run_rr(&s, quantum);
    } else if (strcmp(algorithm, "MLFQ") == 0) {
        run_mlfq(&s);
    } else {
        printf("Unknown algorithm: %s\n", algorithm);
        return -1;
    }

    calculate_metrics(processes, n);
    return 0;
}