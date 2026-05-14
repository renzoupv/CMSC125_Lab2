#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

static int all_processes_complete(SchedulerState *s) {
    for (int i = 0; i < s->n; i++) {
        if (s->processes[i].remaining_time > 0) return 0;
    }
    return 1;
}

void simulate_scheduler(SchedulerState *s, char *algorithm, int quantum) {
    int time = 0;
    Process *current_process = NULL;
    Process *last_process = NULL;

    // Reset process states for simulation
    for (int i = 0; i < s->n; i++) {
        s->processes[i].remaining_time = s->processes[i].burst_time;
        s->processes[i].start_time = -1;
        s->processes[i].finish_time = -1;
        s->processes[i].waiting_time = 0;
        s->processes[i].turnaround_time = 0;
        s->processes[i].response_time = 0;
    }

    while (!all_processes_complete(s)) {
        s->time = time;

        // 1. Check for arrivals (Event-like logic)
        for (int i = 0; i < s->n; i++) {
            if (s->processes[i].arrival_time == time) {
                // In a true event engine, we'd push an EVENT_ARRIVAL to a queue.
                // Here we let the algorithm-specific logic handle the new arrival.
            }
        }

        // 2. Select next process using algorithm hook
        Process *next = NULL;
        if (strcmp(algorithm, "FCFS") == 0) next = select_fcfs(s);
        else if (strcmp(algorithm, "SJF") == 0) next = select_sjf(s);
        else if (strcmp(algorithm, "STCF") == 0) next = select_stcf(s);
        else if (strcmp(algorithm, "RR") == 0) next = select_rr(s, quantum);
        else if (strcmp(algorithm, "MLFQ") == 0) next = select_mlfq(s);

        current_process = next;

        // 3. Handle Context Switch
        if (current_process != NULL && current_process != last_process) {
            if (last_process != NULL) s->context_switches++;
        }

        // 4. Run process for 1 time unit
        if (current_process != NULL) {
            if (current_process->start_time == -1) {
                current_process->start_time = time;
            }

            add_gantt_entry(s->gantt, current_process->pid, time + 1);
            current_process->remaining_time--;

            if (current_process->remaining_time == 0) {
                current_process->finish_time = time + 1;
            }
        } else {
            // Idle time
            add_gantt_entry(s->gantt, "IDLE", time + 1);
        }

        last_process = current_process;
        time++;

        if (time > 100000) { // Safety break
            printf("Simulation exceeded maximum time.\n");
            break;
        }
    }
    s->time = time;
}
