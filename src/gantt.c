#include <stdio.h>
#include <string.h>
#include "gantt.h"

void init_gantt(GanttChart *g) {
    g->count = 0;
}

void add_gantt_entry(GanttChart *g, char *pid, int time) {
    if (g->count > 0 && strcmp(g->pids[g->count - 1], pid) == 0) {
        g->times[g->count - 1] = time; // Update end time of current block
        return;
    }
    if (g->count < MAX_TIMELINE) {
        strcpy(g->pids[g->count], pid);
        g->times[g->count] = time;
        g->count++;
    }
}

void print_gantt_chart(GanttChart *g) {
    if (g->count == 0) return;

    printf("\n=== Gantt Chart ===\n");

    // Print blocks
    for (int i = 0; i < g->count; i++) {
        int duration = (i == 0) ? g->times[0] : (g->times[i] - g->times[i-1]);
        if (duration <= 0) continue;
        
        int scale = (duration + 19) / 20; // Scale: 1 char per 20 units
        printf("[%s", g->pids[i]);
        for (int j = 0; j < scale; j++) printf("-");
        printf("]");
    }
    
    printf("\nTime: 0");
    for (int i = 0; i < g->count; i++) {
        int duration = (i == 0) ? g->times[0] : (g->times[i] - g->times[i-1]);
        if (duration <= 0) continue;
        
        int scale = (duration + 19) / 20;
        int width = strlen(g->pids[i]) + 2 + scale;
        
        // Print spaces to align next marker
        for (int j = 0; j < width - 1; j++) printf(" ");
        printf("%d", g->times[i]);
    }
    printf("\n");
}
