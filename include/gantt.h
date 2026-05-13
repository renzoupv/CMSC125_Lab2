#ifndef GANTT_H
#define GANTT_H

#include "process.h"

#define MAX_TIMELINE 2000

typedef struct {
    char pids[MAX_TIMELINE][16];
    int times[MAX_TIMELINE];
    int count;
} GanttChart;

void init_gantt(GanttChart *g);
void add_gantt_entry(GanttChart *g, char *pid, int time);
void print_gantt_chart(GanttChart *g);

#endif
