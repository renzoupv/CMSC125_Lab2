#include "scheduler.h"
#include <stddef.h>
#include <stdio.h>

Process* select_stcf(SchedulerState *s) {
    Process *selected = NULL;
    int shortest_remaining = 2147483647;
    static Process *last_running = NULL;

    for (int i = 0; i < s->n; i++) {
        if (s->processes[i].remaining_time > 0 && s->processes[i].arrival_time <= s->time) {
            if (s->processes[i].remaining_time < shortest_remaining) {
                shortest_remaining = s->processes[i].remaining_time;
                selected = &s->processes[i];
            } else if (s->processes[i].remaining_time == shortest_remaining) {
                // Tie-breaker: earliest arrival
                if (selected == NULL || s->processes[i].arrival_time < selected->arrival_time) {
                    selected = &s->processes[i];
                }
            }
        }
    }

    // Log preemption if it happens
    if (last_running != NULL && selected != NULL && last_running != selected && last_running->remaining_time > 0) {
        s->log_ptr += snprintf(s->preemption_log + s->log_ptr, 4096 - s->log_ptr,
                               "Process %s was preempted at t=%d (remaining: %d)\n",
                               last_running->pid, s->time, last_running->remaining_time);
    }

    last_running = selected;
    return selected;
}
