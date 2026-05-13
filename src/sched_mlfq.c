#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "scheduler.h"

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

static int is_empty(Queue *q) {
    return q->front == q->rear;
}

static void reset_queues(Queue *q0, Queue *q1, Queue *q2,
                  Process *processes, int n, int time, SchedulerState *s) {

    printf("t=%d:  Priority boost: all processes -> Q0\n", time);

    init_queue(q0);
    init_queue(q1);
    init_queue(q2);

    for (int i = 0; i < n; i++) {
        if (processes[i].remaining_time > 0) {
            processes[i].priority = 0;
            processes[i].allotment_remaining = s->q0_allotment;
            enqueue(q0, &processes[i]);
        }
    }
}

void run_mlfq(SchedulerState *s) {
    Process *p = s->processes;
    int n = s->n;
    Queue q0, q1, q2;
    init_queue(&q0);
    init_queue(&q1);
    init_queue(&q2);

    int time = 0;
    int last_boost = 0;
    Process *last_p = NULL;

    printf("\n=== MLFQ Configuration ===\n");
    printf("Queue 0: q=%d, allotment=%d (highest priority)\n", s->q0_quantum, s->q0_allotment);
    printf("Queue 1: q=%d, allotment=%d\n", s->q1_quantum, s->q1_allotment);
    printf("Queue 2: FCFS (lowest priority)\n");
    printf("Boost period: %d\n", s->boost_period);

    printf("\n=== Execution Trace ===\n");

    // init
    bool enqueued[MAX_PROCESSES] = {false};
    for (int i = 0; i < n; i++) {
        p[i].priority = 0;
        p[i].allotment_remaining = s->q0_allotment;
        p[i].start_time = -1;
    }

    while (1) {
        // ... (completion check) ...
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time > 0) {
                done = 0;
                break;
            }
        }
        if (done) break;

        // check for arrivals
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time && !enqueued[i]) {
                p[i].priority = 0;
                p[i].allotment_remaining = s->q0_allotment;
                enqueue(&q0, &p[i]);
                enqueued[i] = true;
                printf("t=%d:   Process %s enters Q0\n", time, p[i].pid);
            }
        }

        // priority boost
        if (time - last_boost >= s->boost_period && time > 0) {
            reset_queues(&q0, &q1, &q2, p, n, time, s);
            last_boost = time;
        }

        Process *current = NULL;
        int quantum = 0;

        // pick queue
        if (!is_empty(&q0)) {
            current = dequeue(&q0);
            quantum = s->q0_quantum;
        }
        else if (!is_empty(&q1)) {
            current = dequeue(&q1);
            quantum = s->q1_quantum;
        }
        else if (!is_empty(&q2)) {
            current = dequeue(&q2);
            quantum = 1000000; // FCFS runs until completion
        }

        if (current == NULL) {
            time++;
            continue;
        }

        if (last_p != NULL && last_p != current) {
            s->context_switches++;
        }

        if (current->start_time == -1) {
            current->start_time = time;
        }

        int run_time = 0;
        while (run_time < quantum && current->remaining_time > 0) {
            current->remaining_time--;
            current->allotment_remaining--;
            time++;
            run_time++;
            add_gantt_entry(s->gantt, current->pid, time);

            // check for new arrivals (preemption)
            bool preempted = false;
            for (int i = 0; i < n; i++) {
                if (p[i].arrival_time <= time && !enqueued[i]) {
                    p[i].priority = 0;
                    p[i].allotment_remaining = s->q0_allotment;
                    enqueue(&q0, &p[i]);
                    enqueued[i] = true;
                    printf("t=%d:   Process %s enters Q0\n", time, p[i].pid);
                    if (current->priority > 0) {
                        preempted = true;
                    }
                }
            }

            if (time - last_boost >= s->boost_period) break;
            if (preempted) break;
        }

        last_p = current;

        if (current->remaining_time == 0) {
            current->finish_time = time;
            printf("t=%d:  Process %s completes in Q%d\n", time, current->pid, current->priority);
            continue;
        }

        // Demotion / Re-enqueue
        if (current->allotment_remaining <= 0) {
            if (current->priority == 0) {
                current->priority = 1;
                current->allotment_remaining = s->q1_allotment;
                enqueue(&q1, current);
                printf("t=%d:  Process %s -> Q1 (exhausted Q0 allotment)\n", time, current->pid);
            }
            else if (current->priority == 1) {
                current->priority = 2;
                current->allotment_remaining = 1000000; // Infinite for Q2
                enqueue(&q2, current);
                printf("t=%d:  Process %s -> Q2 (exhausted Q1 allotment)\n", time, current->pid);
            }
            else {
                enqueue(&q2, current);
            }
        } else {
            // Put back in same queue (preempted or boost)
            if (current->priority == 0) enqueue(&q0, current);
            else if (current->priority == 1) enqueue(&q1, current);
            else enqueue(&q2, current);
        }
    }
    s->time = time;
}
