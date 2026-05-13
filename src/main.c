#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"
#include "metrics.h"

#define MAX_PROCESSES 100

int main(int argc, char *argv[]) {

    Process processes[MAX_PROCESSES];
    int n = 0;

    char *algorithm = NULL;
    int quantum = 30;
    char *input_file = NULL;
    char *cli_processes = NULL;

    // ----------------------------
    // ARGUMENT PARSING
    // ----------------------------
    for (int i = 1; i < argc; i++) {

        if (strncmp(argv[i], "--algorithm=", 12) == 0) {
            algorithm = argv[i] + 12;
        }

        else if (strncmp(argv[i], "--quantum=", 10) == 0) {
            quantum = atoi(argv[i] + 10);
        }

        else if (strncmp(argv[i], "--input=", 8) == 0) {
            input_file = argv[i] + 8;
        }

        else if (strncmp(argv[i], "--processes=", 13) == 0) {
            cli_processes = argv[i] + 13;
        }
    }

    // ----------------------------
    // LOAD INPUT
    // ----------------------------
    if (input_file != NULL) {
        n = parse_file(input_file, processes);
    }
    else if (cli_processes != NULL) {
        n = parse_cli(cli_processes, processes);
    }
    else {
        printf("Error: No input provided\n");
        return 1;
    }

    if (n <= 0) {
        printf("Error: No processes loaded\n");
        return 1;
    }

    // ----------------------------
    // RUN SCHEDULER
    // ----------------------------
    printf("\nRunning %s Scheduler...\n", algorithm);

    if (schedule(processes, n, algorithm, quantum) != 0) {
        printf("Scheduling failed\n");
        return 1;
    }

    // ----------------------------
    // METRICS OUTPUT
    // ----------------------------
    printf("\n=== Metrics Summary ===\n");

    for (int i = 0; i < n; i++) {
        printf("Process %s | TT=%d | WT=%d | RT=%d\n",
               processes[i].pid,
               processes[i].turnaround_time,
               processes[i].waiting_time,
               processes[i].response_time);
    }

    print_average_metrics(processes, n);

    return 0;
}