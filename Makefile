MONITOR = monitor.out
CC = gcc
M_CFLAGS = \
	   -Wall -O2 -pthread -lncurses

LDFLAGS = \
	  -lm -pthread -lncurses
OBJS = \
	display.o \
	cpu.o \
	monitor.o

%.o: %.c
	$(CC) $(M_CFLAGS) -c -o $@ $<



all:
	gcc -o cpu_load.out main.c -O2 -Wall

monitor: $(OBJS)
	$(CC) -o $(MONITOR) $^ $(LDFLAGS)

clean:
	rm *.o
	rm -f *.out

run:
	make clean
	make all
	./cpu_load.out


