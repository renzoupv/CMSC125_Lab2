#include "scheduler.h"
#include <stddef.h>

Process* select_sjf(SchedulerState *s) {
    static Process *current = NULL;

    // Non-preemptive: keep running current until done
    if (current != NULL && current->remaining_time > 0) {
        return current;
    }

    Process *selected = NULL;
    int shortest_burst = 2147483647;

    for (int i = 0; i < s->n; i++) {
        if (s->processes[i].remaining_time > 0 && s->processes[i].arrival_time <= s->time) {
            if (s->processes[i].burst_time < shortest_burst) {
                shortest_burst = s->processes[i].burst_time;
                selected = &s->processes[i];
            } else if (s->processes[i].burst_time == shortest_burst) {
                // Tie-breaker: earliest arrival
                if (selected == NULL || s->processes[i].arrival_time < selected->arrival_time) {
                    selected = &s->processes[i];
                }
            }
        }
    }

    current = selected;
    return selected;
}
