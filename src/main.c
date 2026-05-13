#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "process.h"

void compute_all(Process *p, int n) {
    for (int i = 0; i < n; i++) {
        p[i].turnaround_time = p[i].finish_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        p[i].response_time = p[i].start_time - p[i].arrival_time;
    }
}

int main(int argc, char **argv) {
    Process processes[5];

    init_process(&processes[0], "A", 0, 240);
    init_process(&processes[1], "B", 10, 180);
    init_process(&processes[2], "C", 20, 150);
    init_process(&processes[3], "D", 25, 80);
    init_process(&processes[4], "E", 30, 130);

    SchedulerState state = {processes, 5, 0};

    run_fcfs(&state);

    compute_all(processes, 5);

    printf("=== FCFS DONE ===\n");
    for (int i = 0; i < 5; i++) {
        printf("%s TT=%d WT=%d RT=%d\n",
            processes[i].pid,
            processes[i].turnaround_time,
            processes[i].waiting_time,
            processes[i].response_time);
    }

    return 0;
}