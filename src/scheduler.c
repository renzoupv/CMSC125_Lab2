#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "metrics.h"

int schedule(Process *processes, int n, char *algorithm, int quantum) {
    SchedulerState s;
    GanttChart g;
    init_gantt(&g);

    s.processes = processes;
    s.n = n;
    s.time = 0;
    s.gantt = &g;

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

    print_gantt_chart(&g);
    calculate_metrics(processes, n);
    return 0;
}