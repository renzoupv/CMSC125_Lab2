#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "process.h"

#define Q0_Q 10
#define Q1_Q 30
#define BOOST_INTERVAL 50

typedef struct {
    Process *queue[100];
    int front, rear;
} Queue;

void init_queue(Queue *q) {
    q->front = q->rear = 0;
}

void enqueue(Queue *q, Process *p) {
    q->queue[q->rear++] = p;
}

Process *dequeue(Queue *q) {
    if (q->front == q->rear) return NULL;
    return q->queue[q->front++];
}

int is_empty(Queue *q) {
    return q->front == q->rear;
}

// reset all queues (used for boost)
void reset_queues(Queue *q0, Queue *q1, Queue *q2,
                  Process *processes, int n, int time) {

    printf("[BOOST at %d] Priority reset\n", time);

    init_queue(q0);
    init_queue(q1);
    init_queue(q2);

    for (int i = 0; i < n; i++) {
        if (processes[i].remaining_time > 0) {
            processes[i].priority = 0;
            enqueue(q0, &processes[i]);
        }
    }
}

int run_mlfq(Process *p, int n) {

    Queue q0, q1, q2;
    init_queue(&q0);
    init_queue(&q1);
    init_queue(&q2);

    int time = 0;
    int last_boost = 0;

    // init
    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].priority = 0;
        p[i].start_time = -1;
    }

    // enqueue all initially arrived processes
    for (int i = 0; i < n; i++) {
        enqueue(&q0, &p[i]);
    }

    while (1) {

        // check completion
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time > 0) {
                done = 0;
                break;
            }
        }
        if (done) break;

        // priority boost
        if (time - last_boost >= BOOST_INTERVAL) {
            reset_queues(&q0, &q1, &q2, p, n, time);
            last_boost = time;
        }

        Process *current = NULL;
        int quantum = 0;

        // pick queue
        if (!is_empty(&q0)) {
            current = dequeue(&q0);
            quantum = Q0_Q;
        }
        else if (!is_empty(&q1)) {
            current = dequeue(&q1);
            quantum = Q1_Q;
        }
        else if (!is_empty(&q2)) {
            current = dequeue(&q2);
            quantum = current->remaining_time;
        }

        if (current == NULL) {
            time++;
            continue;
        }

        // response time
        if (current->start_time == -1) {
            current->start_time = time;
            current->response_time = time - current->arrival_time;
        }

        int run_time = 0;

        while (run_time < quantum && current->remaining_time > 0) {

            printf("[t=%d] Running %s (Q%d)\n",
                   time, current->pid, current->priority);

            current->remaining_time--;
            time++;
            run_time++;

            if (time - last_boost >= BOOST_INTERVAL)
                break;
        }

        // finished
        if (current->remaining_time == 0) {
            current->finish_time = time;
            current->turnaround_time = time - current->arrival_time;
            current->waiting_time =
                current->turnaround_time - current->burst_time;
            continue;
        }

        // demotion logic
        if (current->priority == 0) {
            current->priority = 1;
            enqueue(&q1, current);
        }
        else if (current->priority == 1) {
            current->priority = 2;
            enqueue(&q2, current);
        }
        else {
            enqueue(&q2, current);
        }
    }

    printf("\n=== MLFQ DONE ===\n");
    for (int i = 0; i < n; i++) {
        printf("%s TT=%d WT=%d RT=%d\n",
               p[i].pid,
               p[i].turnaround_time,
               p[i].waiting_time,
               p[i].response_time);
    }

    return 0;
}