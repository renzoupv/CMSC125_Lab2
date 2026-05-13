CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src

mysh: $(SRC)/main.c $(SRC)/process.c $(SRC)/sched_fcfs.c $(SRC)/metrics.c
	$(CC) $(CFLAGS) $^ -o schedsim

clean:
	rm -f schedsim