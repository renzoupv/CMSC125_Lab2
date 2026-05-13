#include "scheduler.h"
#include <stdio.h>

void run_rr(SchedulerState *s, int q) {
    int time = 0;
    int done;
    int last_idx = -1;
    s->context_switches = 0;

    while (1) {
        done = 1;
        int any_ready = 0;

        for (int i = 0; i < s->n; i++) {
            if (s->processes[i].remaining_time > 0) {
                done = 0;
                
                if (s->processes[i].arrival_time <= time) {
                    any_ready = 1;
                    
                    if (last_idx != -1 && last_idx != i) {
                        s->context_switches++;
                    }

                    if (s->processes[i].start_time == -1)
                        s->processes[i].start_time = time;

                    int slice = q;
                    while (slice-- > 0 && s->processes[i].remaining_time > 0) {
                        s->processes[i].remaining_time--;
                        time++;
                        add_gantt_entry(s->gantt, s->processes[i].pid, time);
                    }

                    if (s->processes[i].remaining_time == 0)
                        s->processes[i].finish_time = time;
                        
                    last_idx = i;
                }
            }
        }

        if (done) break;
        
        if (!any_ready) {
            time++;
        }
    }
    s->time = time;
}