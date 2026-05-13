#include <stdio.h>
#include <stdlib.h>
#include "process.h"

// check if all processes are done
int all_done(Process *p, int n) {
    for (int i = 0; i < n; i++) {
        if (p[i].remaining_time > 0)
            return 0;
    }
    return 1;
}

// simple FCFS simulation engine (baseline)
void simulate_fcfs(Process *p, int n) {

    int time = 0;

    for (int i = 0; i < n; i++) {

        if (time < p[i].arrival_time)
            time = p[i].arrival_time;

        p[i].start_time = time;
        p[i].response_time = time - p[i].arrival_time;

        printf("[Time %d] Running process %s\n", time, p[i].pid);

        time += p[i].burst_time;

        p[i].finish_time = time;
        p[i].turnaround_time = p[i].finish_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
    }
}

// generic time-step simulation (for future STCF / RR / MLFQ)
void simulate_time(Process *p, int n) {

    int time = 0;

    while (!all_done(p, n)) {

        for (int i = 0; i < n; i++) {

            if (p[i].arrival_time == time) {
                printf("[Time %d] Process %s arrived\n", time, p[i].pid);
            }
        }

        time++;

        if (time > 10000) break; // safety guard
    }
}