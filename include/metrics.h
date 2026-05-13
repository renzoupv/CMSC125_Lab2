#ifndef METRICS_H
#define METRICS_H

#include "process.h"

void calculate_metrics(Process *processes, int n);

double avg_turnaround(Process *processes, int n);
double avg_waiting(Process *processes, int n);
double avg_response(Process *processes, int n);

void print_metrics(Process *processes, int n);
void print_average_metrics(Process *processes, int n);

#endif