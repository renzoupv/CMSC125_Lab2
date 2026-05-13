#include <stdio.h>
#include "metrics.h"

void calculate_metrics(Process *processes, int n) {
    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];
        p->turnaround_time = p->finish_time - p->arrival_time;
        p->waiting_time = p->turnaround_time - p->burst_time;
        p->response_time = p->start_time - p->arrival_time;
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

void print_average_metrics(Process *processes, int n) {
    printf("Average |     |     |     | %.1f | %.1f | %.1f\n", 
        avg_turnaround(processes, n),
        avg_waiting(processes, n),
        avg_response(processes, n));
}