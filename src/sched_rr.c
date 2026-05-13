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