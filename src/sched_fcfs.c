#include <stdio.h>
#include "scheduler.h"

static int all_done(SchedulerState *s) {
    for (int i = 0; i < s->n; i++)
        if (s->processes[i].remaining_time > 0)
            return 0;
    return 1;
}

void run_fcfs(SchedulerState *s) {
    int time = 0;

    while (!all_done(s)) {
        int idx = -1;

        for (int i = 0; i < s->n; i++) {
            if (s->processes[i].arrival_time <= time &&
                s->processes[i].remaining_time > 0) {
                idx = i;
                break;
            }
        }

        if (idx != -1) {
            Process *p = &s->processes[idx];

            if (p->start_time == -1)
                p->start_time = time;

            p->remaining_time--;
            add_gantt_entry(s->gantt, p->pid, time + 1);

            if (p->remaining_time == 0)
                p->finish_time = time + 1;
        }

        time++;
    }
}