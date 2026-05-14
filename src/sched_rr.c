#include "scheduler.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    Process *queue[MAX_PROCESSES];
    int front, rear;
    int current_quantum;
    bool enqueued[MAX_PROCESSES];
} RRData;

Process* select_rr(SchedulerState *s, int q) {
    if (s->alg_data == NULL) {
        s->alg_data = calloc(1, sizeof(RRData));
        RRData *data = (RRData*)s->alg_data;
        data->current_quantum = 0;
    }
    RRData *data = (RRData*)s->alg_data;

    // 1. Check for new arrivals
    for (int i = 0; i < s->n; i++) {
        if (s->processes[i].arrival_time <= s->time && !data->enqueued[i]) {
            data->queue[data->rear++] = &s->processes[i];
            data->enqueued[i] = true;
        }
    }

    static Process *current = NULL;

    // 2. If quantum expired or process finished, pick next
    if (current == NULL || current->remaining_time == 0 || data->current_quantum >= q) {
        
        // If quantum expired but not finished, put back at end
        if (current != NULL && current->remaining_time > 0) {
            data->queue[data->rear++] = current;
        }

        if (data->front == data->rear) {
            current = NULL;
        } else {
            current = data->queue[data->front++];
        }
        data->current_quantum = 0;
    }

    if (current != NULL) {
        data->current_quantum++;
    }

    return current;
}
