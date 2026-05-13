#include <string.h>
#include "process.h"

void init_process(Process *p, char *pid, int at, int bt) {
    strcpy(p->pid, pid);
    p->arrival_time = at;
    p->burst_time = bt;
    p->remaining_time = bt;

    p->start_time = -1;
    p->finish_time = -1;
    p->response_time = -1;
    p->waiting_time = 0;
    p->turnaround_time = 0;

    p->priority = 0;
    p->time_in_queue = 0;
}