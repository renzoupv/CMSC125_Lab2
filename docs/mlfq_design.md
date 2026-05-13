# MLFQ Design Justification

## 1. Number of Queues
Our MLFQ implementation uses **3 priority levels**. This design follows the classic multi-level feedback queue approach, providing enough granularity to distinguish between highly interactive jobs, mixed-workload jobs, and long-running batch processes.
- **Q0 (Highest Priority)**: Reserved for very short, interactive bursts.
- **Q1 (Medium Priority)**: For processes that need more CPU time but still benefit from responsiveness.
- **Q2 (Lowest Priority)**: For long-running, compute-intensive tasks (executed via FCFS).

## 2. Time Quantum per Level
- **Q0: 10 time units**. A small quantum ensures that truly interactive jobs get immediate attention and finish quickly without blocking others.
- **Q1: 30 time units**. A larger quantum for processes that have been demoted, reducing context-switch overhead for medium-length jobs.
- **Q2: FCFS (Infinity)**. Processes in the lowest queue are likely batch jobs, so they run until completion or preemption by higher-priority arrivals.

## 3. Allotment Values
- **Q0 Allotment: 50 time units**. This allows a process to stay in the high-priority queue for up to 5 quantum cycles if it yields frequently (interactive behavior). If it exhausts 50 units, it is demoted to Q1.
- **Q1 Allotment: 150 time units**. This provides a substantial "buffer" for mixed workloads. Only truly long-running processes (exceeding 200 total units) will sink to the lowest priority (Q2).

## 4. Boost Period
- **Boost Period (S): 200 time units**. We chose 200 units to periodically reset priorities and prevent starvation of long-running jobs in Q2. This value was balanced against the default allotments to ensure that processes have a chance to re-prove their interactive nature regularly.

## 5. Empirical Testing
During testing with the `quiz3.txt` workload (total duration ~800 units), we observed that interactive processes (like Process D with burst 80) completed while mostly staying in Q0/Q1, while long-running processes (Process A with burst 240) were correctly demoted to Q2. The boost period of 200 ensured that Process A didn't wait indefinitely once shorter jobs finished. This configuration provided a good balance between average turnaround time and responsiveness for interactive-like bursts.
