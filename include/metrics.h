#ifndef METRICS_H
#define METRICS_H

#include "process.h"

void calculate_metrics(Process processes, int n);

double avg_turnaround(Processprocesses, int n);
double avg_waiting(Process processes, int n);
double avg_response(Processprocesses, int n);

void print_metrics(Process *processes, int n);

#endif
gantt.h:

#ifndef GANTT_H
#define GANTT_H

#include "process.h"

void init_gantt(int total_time);
void record_gantt(int time, const char *pid);
void print_gantt(int total_time);

#endif