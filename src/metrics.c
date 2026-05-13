#include <stdio.h>
#include <string.h>
#include "metrics.h"

void calculate_metrics(Process *processes, int n) {
    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];
        if (p->finish_time == -1) {
            p->turnaround_time = 0;
            p->waiting_time = 0;
            p->response_time = 0;
        } else {
            p->turnaround_time = p->finish_time - p->arrival_time;
            p->waiting_time = p->turnaround_time - p->burst_time;
            p->response_time = p->start_time - p->arrival_time;
        }
    }
}

double avg_turnaround(Process *processes, int n) {
    if (n == 0) return 0;
    double sum = 0;
    for (int i = 0; i < n; i++) sum += processes[i].turnaround_time;
    return sum / n;
}

double avg_waiting(Process *processes, int n) {
    if (n == 0) return 0;
    double sum = 0;
    for (int i = 0; i < n; i++) sum += processes[i].waiting_time;
    return sum / n;
}

double avg_response(Process *processes, int n) {
    if (n == 0) return 0;
    double sum = 0;
    for (int i = 0; i < n; i++) sum += processes[i].response_time;
    return sum / n;
}

void print_metrics(Process *processes, int n) {
    printf("\n=== Metrics ===\n");
    printf("Process | AT  | BT  | FT  | TT  | WT  | RT  \n");
    printf("--------|-----|-----|-----|-----|-----|-----\n");
    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];
        printf("%-7s | %3d | %3d | %3d | %3d | %3d | %3d\n",
               p->pid, p->arrival_time, p->burst_time, p->finish_time,
               p->turnaround_time, p->waiting_time, p->response_time);
    }
    printf("--------|-----|-----|-----|-----|-----|-----\n");
    printf("Average |     |     |     | %3.0f | %3.0f | %3.0f\n",
           avg_turnaround(processes, n),
           avg_waiting(processes, n),
           avg_response(processes, n));
}

void check_convoy_effect(Process *processes, int n) {
    // A simple heuristic for convoy effect: a long process followed by short ones
    if (n < 2) return;
    if (processes[0].burst_time > 100) {
        for (int i = 1; i < n; i++) {
            if (processes[i].waiting_time > 100) {
                printf("\nConvoy effect detected: Process %s waited %d time units\n",
                       processes[i].pid, processes[i].waiting_time);
                return;
            }
        }
    }
}
