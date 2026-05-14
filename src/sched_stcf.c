#include "scheduler.h"
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

Process* select_stcf(SchedulerState *s) {
    Process *selected = NULL;
    int shortest_remaining = 2147483647;
    static Process *last_running = NULL;
    static bool is_preempted[MAX_PROCESSES] = {false};

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

    // Log preemption or resumption
    if (last_running != NULL && selected != NULL && last_running != selected) {
        if (last_running->remaining_time > 0) {
            s->log_ptr += snprintf(s->preemption_log + s->log_ptr, 4096 - s->log_ptr,
                                   "Process %s was preempted at t=%d (remaining: %d)\n",
                                   last_running->pid, s->time, last_running->remaining_time);
            
            // Mark last_running as preempted
            for (int i = 0; i < s->n; i++) {
                if (&s->processes[i] == last_running) {
                    is_preempted[i] = true;
                    break;
                }
            }
        }

        // Check if selected was previously preempted
        for (int i = 0; i < s->n; i++) {
            if (&s->processes[i] == selected) {
                if (is_preempted[i]) {
                    s->log_ptr += snprintf(s->preemption_log + s->log_ptr, 4096 - s->log_ptr,
                                           "Process %s resumed at t=%d\n",
                                           selected->pid, s->time);
                    is_preempted[i] = false;
                }
                break;
            }
        }
    }

    last_running = selected;
    return selected;
}
