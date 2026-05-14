#include "scheduler.h"
#include <stddef.h>

Process* select_fcfs(SchedulerState *s) {
    Process *selected = NULL;
    int earliest_arrival = 2147483647;

    // Find the process that arrived earliest and is not yet finished
    for (int i = 0; i < s->n; i++) {
        if (s->processes[i].remaining_time > 0 && s->processes[i].arrival_time <= s->time) {
            if (s->processes[i].arrival_time < earliest_arrival) {
                earliest_arrival = s->processes[i].arrival_time;
                selected = &s->processes[i];
            }
        }
    }

    // FCFS is non-preemptive, so if we were running something, keep running it
    // However, in this simple selection logic, if we always pick the one with earliest_arrival,
    // it will naturally stay with the current one until it's done because no one can have an
    // arrival time earlier than the current running one (unless they arrived at the same time).
    // To be safe and explicitly non-preemptive:
    static Process *current = NULL;
    if (current != NULL && current->remaining_time > 0) {
        return current;
    }

    current = selected;
    return selected;
}
