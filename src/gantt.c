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

    int block_widths[MAX_TIMELINE];
    int total_width = 0;

    // Print blocks and calculate widths
    for (int i = 0; i < g->count; i++) {
        int duration = (i == 0) ? g->times[0] : (g->times[i] - g->times[i-1]);
        if (duration <= 0) {
            block_widths[i] = 0;
            continue;
        }
        
        int scale = (duration + 9) / 10; 
        block_widths[i] = strlen(g->pids[i]) + 2 + scale;
        printf("[%s", g->pids[i]);
        for (int j = 0; j < scale; j++) printf("-");
        printf("]");
    }
    
    printf("\nTime: 0");
    int current_pos = 7; // "Time: 0" ends at pos 7 (0 is at pos 6)
    
    for (int i = 0; i < g->count; i++) {
        if (block_widths[i] == 0) continue;
        
        total_width += block_widths[i];
        
        char marker[16];
        sprintf(marker, "%d", g->times[i]);
        int marker_len = strlen(marker);
        
        int target_pos = total_width;
        int spaces_needed = target_pos - current_pos;
        
        if (spaces_needed < 0) spaces_needed = 1; // Safety
        
        for (int j = 0; j < spaces_needed; j++) printf(" ");
        printf("%s", marker);
        current_pos = target_pos + marker_len;
    }
    printf("\n");
}
