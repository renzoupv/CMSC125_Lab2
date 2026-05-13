#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "gantt.h"

typedef struct {
    Process *processes;
    int n;
    int time;
    GanttChart *gantt;
    int context_switches;
    
    // MLFQ Config
    int q0_quantum, q0_allotment;
    int q1_quantum, q1_allotment;
    int boost_period;
    
    char preemption_log[4096];
    int log_ptr;
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