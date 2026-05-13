#include "scheduler.h"

static int pick_sjf(SchedulerState *s, int time) {
    int idx = -1;

    for (int i = 0; i < s->n; i++) {
        if (s->processes[i].arrival_time <= time &&
            s->processes[i].remaining_time > 0) {

            if (idx == -1 ||
                s->processes[i].burst_time < s->processes[idx].burst_time) {
                idx = i;
            }
        }
    }
    return idx;
}

void run_sjf(SchedulerState *s) {
    int time = 0;

    for (;;) {
        int done = 1;
        for (int i = 0; i < s->n; i++)
            if (s->processes[i].remaining_time > 0)
                done = 0;

        if (done) break;

        int idx = pick_sjf(s, time);

        if (idx == -1) {
            time++;
            continue;
        }

        Process *p = &s->processes[idx];

        if (p->start_time == -1)
            p->start_time = time;

        while (p->remaining_time > 0) {
            p->remaining_time--;
            time++;
            add_gantt_entry(s->gantt, p->pid, time);
        }

        p->finish_time = time;
    }
}