#include <stdio.h>
#include "process.h"

// simple gantt chart from execution log
void print_gantt(char timeline[], int size) {

    printf("\n=== Gantt Chart ===\n");

    // top bar
    for (int i = 0; i < size; i++) {
        printf("[%c]", timeline[i]);
    }

    printf("\n0");

    // time markers
    for (int i = 10; i < size; i += 10) {
        printf("   %d", i);
    }

    printf("\n");
}