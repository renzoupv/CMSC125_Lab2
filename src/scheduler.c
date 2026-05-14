#include <stdio.h>
#include <string.h>
#include "scheduler.h"
#include "metrics.h"

int schedule(Process *processes, int n, char *algorithm, int quantum) {
    SchedulerState s;
    GanttChart g;
    init_gantt(&g);

    s.processes = processes;
    s.n = n;
    s.time = 0;
    s.gantt = &g;
    s.context_switches = 0;
    s.log_ptr = 0;
    s.preemption_log[0] = '\0';
    s.alg_data = NULL;

    // MLFQ Defaults if needed
    s.q0_quantum = 10; s.q0_allotment = 50;
    s.q1_quantum = 30; s.q1_allotment = 150;
    s.boost_period = 200;

    simulate_scheduler(&s, algorithm, quantum);

    print_gantt_chart(&g);
    calculate_metrics(processes, n);
    return 0;
}
