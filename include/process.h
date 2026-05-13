#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char pid[16];
    int arrival_time;
    int burst_time;
    int remaining_time;

    int start_time;
    int finish_time;

    int waiting_time;
    int turnaround_time;
    int response_time;

    int priority;
    int time_in_queue;
} Process;

void init_process(Process *p, char *pid, int at, int bt);

#endif