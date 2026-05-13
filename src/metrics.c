#include "process.h"

void compute_metrics(Process *p) {
    p->turnaround_time = p->finish_time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->burst_time;
    p->response_time = p->start_time - p->arrival_time;
}