#include "scheduler.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    Process *queues[3][MAX_PROCESSES];
    int front[3], rear[3];
    int current_quantum;
    int last_boost;
    bool enqueued[MAX_PROCESSES];
} MLFQData;

static void mlfq_enqueue(MLFQData *data, int q_idx, Process *p) {
    data->queues[q_idx][data->rear[q_idx]++] = p;
}

static Process* mlfq_dequeue(MLFQData *data, int q_idx) {
    if (data->front[q_idx] == data->rear[q_idx]) return NULL;
    return data->queues[q_idx][data->front[q_idx]++];
}

Process* select_mlfq(SchedulerState *s) {
    if (s->alg_data == NULL) {
        s->alg_data = calloc(1, sizeof(MLFQData));
        MLFQData *data = (MLFQData*)s->alg_data;
        data->last_boost = 0;
        printf("\n=== MLFQ Configuration ===\n");
        printf("Queue 0: q=%d, allotment=%d (highest priority)\n", s->q0_quantum, s->q0_allotment);
        printf("Queue 1: q=%d, allotment=%d\n", s->q1_quantum, s->q1_allotment);
        printf("Queue 2: FCFS (lowest priority)\n");
        printf("Boost period: %d\n", s->boost_period);
        printf("\n=== Execution Trace ===\n");
    }
    MLFQData *data = (MLFQData*)s->alg_data;

    // 1. Priority Boost
    if (s->time > 0 && s->time - data->last_boost >= s->boost_period) {
        printf("t=%d:  Priority boost: all processes -> Q0\n", s->time);
        for (int i = 0; i < s->n; i++) {
            if (s->processes[i].remaining_time > 0) {
                s->processes[i].priority = 0;
                s->processes[i].allotment_remaining = s->q0_allotment;
            }
        }
        // Re-initialize queues to Q0
        for (int q = 0; q < 3; q++) { data->front[q] = data->rear[q] = 0; }
        for (int i = 0; i < s->n; i++) {
            if (s->processes[i].remaining_time > 0) {
                mlfq_enqueue(data, 0, &s->processes[i]);
                data->enqueued[i] = true;
            }
        }
        data->last_boost = s->time;
        data->current_quantum = 0;
    }

    // 2. Check for new arrivals
    for (int i = 0; i < s->n; i++) {
        if (s->processes[i].arrival_time <= s->time && !data->enqueued[i]) {
            s->processes[i].priority = 0;
            s->processes[i].allotment_remaining = s->q0_allotment;
            mlfq_enqueue(data, 0, &s->processes[i]);
            data->enqueued[i] = true;
            printf("t=%d:   Process %s enters Q0\n", s->time, s->processes[i].pid);
        }
    }

    static Process *current = NULL;

    // 3. Determine if current process should continue
    int q = (current && current->priority == 0) ? s->q0_quantum : 
            (current && current->priority == 1) ? s->q1_quantum : 1000000;

    bool switch_needed = false;
    if (current == NULL || current->remaining_time == 0) switch_needed = true;
    else if (data->current_quantum >= q) switch_needed = true;
    else if (current->allotment_remaining <= 0) switch_needed = true;
    else {
        // Preemption check: if something is in a higher queue
        for (int i = 0; i < current->priority; i++) {
            if (data->front[i] != data->rear[i]) {
                switch_needed = true;
                break;
            }
        }
    }

    if (switch_needed) {
        if (current != NULL) {
            if (current->remaining_time == 0) {
                printf("t=%d:  Process %s completes in Q%d\n", s->time, current->pid, current->priority);
            } else if (current->allotment_remaining <= 0) {
                if (current->priority == 0) {
                    current->priority = 1;
                    current->allotment_remaining = s->q1_allotment;
                    mlfq_enqueue(data, 1, current);
                    printf("t=%d:  Process %s -> Q1 (exhausted Q0 allotment)\n", s->time, current->pid);
                } else if (current->priority == 1) {
                    current->priority = 2;
                    current->allotment_remaining = 1000000;
                    mlfq_enqueue(data, 2, current);
                    printf("t=%d:  Process %s -> Q2 (exhausted Q1 allotment)\n", s->time, current->pid);
                } else {
                    mlfq_enqueue(data, 2, current);
                }
            } else {
                // Preempted or quantum expired, re-enqueue in same priority
                mlfq_enqueue(data, current->priority, current);
            }
        }

        // Pick next
        current = NULL;
        for (int i = 0; i < 3; i++) {
            current = mlfq_dequeue(data, i);
            if (current != NULL) break;
        }
        data->current_quantum = 0;
    }

    if (current != NULL) {
        data->current_quantum++;
        current->allotment_remaining--;
    }

    return current;
}
