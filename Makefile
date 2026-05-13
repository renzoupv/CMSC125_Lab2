CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRCS = src/main.c \
       src/process.c \
       src/parser.c \
       src/scheduler.c \
       src/sched_fcfs.c \
       src/sched_sjf.c \
       src/sched_stcf.c \
       src/sched_rr.c \
       src/sched_mlfq.c \
       src/simulation.c \
       src/gantt.c \
       src/metrics.c \
       src/utils.c

OBJS = $(SRCS:.c=.o)

schedsim: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f schedsim $(OBJS)