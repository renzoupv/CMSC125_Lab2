mlfq.h:

#ifndef MLFQ_H
#define MLFQ_H

#include "process.h"

#define MAX_Q 10

typedef struct {
    int level;
    int time_quantum;
    int allotment;
} QueueConfig;

typedef struct {
    Process queues[MAX_Q][MAX_PROCESSES];
    int sizes[MAX_Q];

    int num_levels;
    int boost_period;
    int last_boost;
} MLFQScheduler;

void init_mlfq(MLFQSchedulerm, QueueConfig cfg, int levels, int boost);
void mlfq_add_process(MLFQSchedulerm, Process p);
Processmlfq_select(MLFQScheduler m, int current_time);
void mlfq_update(MLFQSchedulerm, Process *p, int time_used);

#endif