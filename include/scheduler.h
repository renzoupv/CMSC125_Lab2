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

    // Algorithm State
    void *alg_data;
} SchedulerState;

typedef enum {
    EVENT_ARRIVAL,
    EVENT_COMPLETION,
    EVENT_QUANTUM_EXPIRE,
    EVENT_PRIORITY_BOOST
} EventType;

typedef struct Event {
    int time;
    EventType type;
    Process *process;
    struct Event *next;
} Event;

int parse_file(char *filename, Process *processes);
int parse_cli(char *arg, Process *processes);
int schedule(Process *processes, int n, char *algorithm, int quantum);

// Generic Simulation Engine
void simulate_scheduler(SchedulerState *state, char *algorithm, int quantum);

// Algorithm Hooks
Process* select_fcfs(SchedulerState *s);
Process* select_sjf(SchedulerState *s);
Process* select_stcf(SchedulerState *s);
Process* select_rr(SchedulerState *s, int q);
Process* select_mlfq(SchedulerState *s);

#endif