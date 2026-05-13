#include <stdio.h>
#include "scheduler.h"

static int pick_stcf(SchedulerState *s, int time) {
    int idx = -1;

    for (int i = 0; i < s->n; i++) {
        if (s->processes[i].arrival_time <= time &&
            s->processes[i].remaining_time > 0) {

            if (idx == -1 ||
                s->processes[i].remaining_time < s->processes[idx].remaining_time) {
                idx = i;
            }
        }
    }
    return idx;
}

void run_stcf(SchedulerState *s) {
    int time = 0;
    int last_idx = -1;
    s->log_ptr = 0;
    s->preemption_log[0] = '\0';

    while (1) {
        int done = 1;
        for (int i = 0; i < s->n; i++)
            if (s->processes[i].remaining_time > 0)
                done = 0;

        if (done) break;

        int idx = pick_stcf(s, time);

        if (idx == -1) {
            time++;
            continue;
        }

        Process *p = &s->processes[idx];

        if (last_idx != -1 && last_idx != idx) {
            s->context_switches++;
            Process *last_p = &s->processes[last_idx];
            if (last_p->remaining_time > 0) {
                s->log_ptr += sprintf(s->preemption_log + s->log_ptr, 
                       "Process %s was preempted at t=%d (remaining: %d)\n",
                       last_p->pid, time, last_p->remaining_time);
            }
            s->log_ptr += sprintf(s->preemption_log + s->log_ptr,
                       "Process %s resumed at t=%d\n", p->pid, time);
        }

        if (p->start_time == -1)
            p->start_time = time;

        p->remaining_time--;
        time++;
        add_gantt_entry(s->gantt, p->pid, time);

        if (p->remaining_time == 0)
            p->finish_time = time;
            
        last_idx = idx;
    }
}