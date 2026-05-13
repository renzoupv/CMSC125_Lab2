#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

typedef struct {
    Process *processes;
    int n;
    int time;
} SchedulerState;

int parse_file(char *filename, Process *processes);
int parse_cli(char *arg, Process *processes);
int schedule(Process *processes, int n, char *algorithm, int quantum);

void run_fcfs(SchedulerState *s);
void run_sjf(SchedulerState *s);
void run_stcf(SchedulerState *s);
void run_rr(SchedulerState *s, int q);
void run_mlfq(SchedulerState *s);

#endif