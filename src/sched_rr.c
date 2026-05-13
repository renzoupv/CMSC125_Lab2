#include "scheduler.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    Process *queue[100];
    int front, rear;
} Queue;

static void init_queue(Queue *q) {
    q->front = q->rear = 0;
}

static void enqueue(Queue *q, Process *p) {
    q->queue[q->rear++] = p;
}

static Process *dequeue(Queue *q) {
    if (q->front == q->rear) return NULL;
    return q->queue[q->front++];
}

void run_rr(SchedulerState *s, int q) {
    int time = 0;
    int finished_count = 0;
    bool enqueued[MAX_PROCESSES] = {false};
    Queue ready_queue;
    init_queue(&ready_queue);
    
    s->context_switches = 0;
    Process *last_p = NULL;

    while (finished_count < s->n) {
        // 1. Check for new arrivals at current time
        for (int i = 0; i < s->n; i++) {
            if (s->processes[i].arrival_time <= time && !enqueued[i]) {
                enqueue(&ready_queue, &s->processes[i]);
                enqueued[i] = true;
            }
        }

        // 2. Pick next process
        Process *current = dequeue(&ready_queue);
        
        if (current == NULL) {
            time++;
            continue;
        }

        // Context switch tracking: happens if we switch from one process to another
        if (last_p != NULL && last_p != current) {
            s->context_switches++;
        }

        if (current->start_time == -1) {
            current->start_time = time;
        }

        // 3. Run for quantum or until completion
        int run_time = 0;
        while (run_time < q && current->remaining_time > 0) {
            current->remaining_time--;
            time++;
            run_time++;
            add_gantt_entry(s->gantt, current->pid, time);

            // 4. Check for arrivals during execution
            for (int i = 0; i < s->n; i++) {
                if (s->processes[i].arrival_time <= time && !enqueued[i]) {
                    enqueue(&ready_queue, &s->processes[i]);
                    enqueued[i] = true;
                }
            }
        }

        last_p = current;

        if (current->remaining_time == 0) {
            current->finish_time = time;
            finished_count++;
        } else {
            // Quantum expired, re-enqueue
            enqueue(&ready_queue, current);
        }
    }
    s->time = time;
}
