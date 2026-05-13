#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

// helper
void init_process(Process *p, char *pid, int at, int bt) {
    strcpy(p->pid, pid);
    p->arrival_time = at;
    p->burst_time = bt;
    p->remaining_time = bt;
    p->start_time = -1;
    p->finish_time = 0;
    p->waiting_time = 0;
    p->turnaround_time = 0;
    p->response_time = 0;
}

int parse_file(char *filename, Process *processes) {

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("file error");
        return -1;
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {

        if (line[0] == '#' || strlen(line) < 3)
            continue;

        char pid[16];
        int at, bt;

        if (sscanf(line, "%s %d %d", pid, &at, &bt) == 3) {
            init_process(&processes[count], pid, at, bt);
            count++;
        }
    }

    fclose(fp);
    return count;
}

int parse_cli(char *arg, Process *processes) {

    int count = 0;
    char *token = strtok(arg, ",");

    while (token != NULL) {

        char pid[16];
        int at, bt;

        if (sscanf(token, "%[^:]:%d:%d", pid, &at, &bt) == 3) {
            init_process(&processes[count], pid, at, bt);
            count++;
        }

        token = strtok(NULL, ",");
    }

    return count;
}