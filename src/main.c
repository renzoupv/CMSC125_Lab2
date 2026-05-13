#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"
#include "metrics.h"

int main(int argc, char *argv[]) {

    Process processes[MAX_PROCESSES];
    int n = 0;

    char *algorithm = NULL;
    int quantum = 30;
    char *input_file = NULL;
    char *cli_processes = NULL;
    char *mlfq_config_file = NULL;
    int compare = 0;

    // Default MLFQ config
    int q0_q = 10, q0_a = 50;
    int q1_q = 30, q1_a = 150;
    int boost = 200;

    // ----------------------------
    // ARGUMENT PARSING
    // ----------------------------
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--compare") == 0) {
            compare = 1;
        }
        else if (strncmp(argv[i], "--algorithm=", 12) == 0) {
            algorithm = argv[i] + 12;
        }
        else if (strncmp(argv[i], "--quantum=", 10) == 0) {
            quantum = atoi(argv[i] + 10);
        }
        else if (strncmp(argv[i], "--input=", 8) == 0) {
            input_file = argv[i] + 8;
        }
        else if (strncmp(argv[i], "--processes=", 12) == 0) {
            cli_processes = argv[i] + 12;
        }
        else if (strncmp(argv[i], "--mlfq-config=", 14) == 0) {
            mlfq_config_file = argv[i] + 14;
        }
    }
    
    // Parse MLFQ config if provided
    if (mlfq_config_file != NULL) {
        FILE *fp = fopen(mlfq_config_file, "r");
        if (fp) {
            char line[128];
            while (fgets(line, sizeof(line), fp)) {
                if (line[0] == '#' || strlen(line) < 3) continue;
                if (strncmp(line, "Q0", 2) == 0) sscanf(line, "Q0 %d %d", &q0_q, &q0_a);
                else if (strncmp(line, "Q1", 2) == 0) sscanf(line, "Q1 %d %d", &q1_q, &q1_a);
                else if (strncmp(line, "BOOST_PERIOD", 12) == 0) sscanf(line, "BOOST_PERIOD %d", &boost);
            }
            fclose(fp);
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
    // COMPARISON MODE
    // ----------------------------
    if (compare) {
        printf("\n=== Algorithm Comparison for %s ===\n", input_file ? input_file : "CLI input");
        printf("\nAlgorithm | Avg TT | Avg WT | Avg RT | Context Switches\n");
        printf("----------|--------|--------|--------|------------------\n");

        char *algs[] = {"FCFS", "SJF", "STCF", "RR", "MLFQ"};
        for (int i = 0; i < 5; i++) {
            Process temp_p[MAX_PROCESSES];
            memcpy(temp_p, processes, sizeof(Process) * n);
            
            SchedulerState ts;
            GanttChart tg;
            init_gantt(&tg);
            ts.processes = temp_p;
            ts.n = n;
            ts.time = 0;
            ts.gantt = &tg;
            ts.q0_quantum = q0_q; ts.q0_allotment = q0_a;
            ts.q1_quantum = q1_q; ts.q1_allotment = q1_a;
            ts.boost_period = boost;
            ts.log_ptr = 0; ts.preemption_log[0] = '\0';
            ts.context_switches = 0;

            if (strcmp(algs[i], "FCFS") == 0) run_fcfs(&ts);
            else if (strcmp(algs[i], "SJF") == 0) run_sjf(&ts);
            else if (strcmp(algs[i], "STCF") == 0) run_stcf(&ts);
            else if (strcmp(algs[i], "RR") == 0) run_rr(&ts, quantum);
            else if (strcmp(algs[i], "MLFQ") == 0) run_mlfq(&ts);

            calculate_metrics(temp_p, n);
            printf("%-9s | %6.1f | %6.1f | %6.1f | %16d\n",
                   algs[i],
                   avg_turnaround(temp_p, n),
                   avg_waiting(temp_p, n),
                   avg_response(temp_p, n),
                   ts.context_switches);
        }
        return 0;
    }

    // ----------------------------
    // SINGLE ALGORITHM MODE
    // ----------------------------
    if (algorithm == NULL) {
        printf("Error: No algorithm specified. Use --algorithm or --compare\n");
        return 1;
    }

    if (strcmp(algorithm, "RR") == 0) {
        printf("\nUsing time quantum q=%d\n", quantum);
    }
    printf("\nRunning %s Scheduler...\n", algorithm);

    SchedulerState s;
    GanttChart g;
    init_gantt(&g);
    s.processes = processes;
    s.n = n;
    s.time = 0;
    s.gantt = &g;
    s.q0_quantum = q0_q; s.q0_allotment = q0_a;
    s.q1_quantum = q1_q; s.q1_allotment = q1_a;
    s.boost_period = boost;
    s.log_ptr = 0; s.preemption_log[0] = '\0';
    s.context_switches = 0;

    int status = -1;
    if (strcmp(algorithm, "FCFS") == 0) { run_fcfs(&s); status = 0; }
    else if (strcmp(algorithm, "SJF") == 0) { run_sjf(&s); status = 0; }
    else if (strcmp(algorithm, "STCF") == 0) { run_stcf(&s); status = 0; }
    else if (strcmp(algorithm, "RR") == 0) { run_rr(&s, quantum); status = 0; }
    else if (strcmp(algorithm, "MLFQ") == 0) { run_mlfq(&s); status = 0; }

    if (status != 0) {
        printf("Scheduling failed: algorithm '%s' not recognized\n", algorithm);
        return 1;
    }

    print_gantt_chart(&g);
    calculate_metrics(processes, n);

    // ----------------------------
    // METRICS OUTPUT
    // ----------------------------
    print_metrics(processes, n);

    if (strcmp(algorithm, "STCF") == 0 && s.preemption_log[0] != '\0') {
        printf("\n%s", s.preemption_log);
    }

    if (strcmp(algorithm, "RR") == 0) {
        printf("\nTotal context switches: %d\n", s.context_switches);
        printf("Average response time: %.1f\n", avg_response(processes, n));
    }
    
    if (strcmp(algorithm, "FCFS") == 0) {
        check_convoy_effect(processes, n);
    }

    return 0;
}
