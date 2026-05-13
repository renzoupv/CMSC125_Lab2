CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

schedsim: \
src/main.c \
src/process.c \
src/sched_fcfs.c \
src/sched_sjf.c \
src/sched_stcf.c \
src/sched_rr.c \
src/metrics.c
	$(CC) $(CFLAGS) $^ -o schedsim

clean:
	rm -f schedsim