#include "scheduler.h"

static int pick_stcf(SchedulerState s, int time) {
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

void run_stcf(SchedulerStates) {
    int time = 0;

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

        if (p->start_time == -1)
            p->start_time = time;

        p->remaining_time--;
        time++;

        if (p->remaining_time == 0)
            p->finish_time = time;
    }
}
sched_rr:

#include "scheduler.h"

void run_rr(SchedulerState *s, int q) {
    int time = 0;
    int done;

    do {
        done = 1;

        for (int i = 0; i < s->n; i++) {
            if (s->processes[i].remaining_time > 0) {

                done = 0;

                int slice = q;

                if (s->processes[i].arrival_time > time)
                    continue;

                if (s->processes[i].start_time == -1)
                    s->processes[i].start_time = time;

                while (slice-- > 0 && s->processes[i].remaining_time > 0) {
                    s->processes[i].remaining_time--;
                    time++;
                }

                if (s->processes[i].remaining_time == 0)
                    s->processes[i].finish_time = time;
            }
        }

    } while (!done);
}