MONITOR = monitor.out
CC = gcc
M_CFLAGS = \
	   -Wall -O2 -pthread -lncurses -Wno-format -g

LDFLAGS = \
	  -lm -pthread -lncurses
OBJS = \
	display.o \
	cpu.o \
	iodisk.o \
	monitor.o

%.o: %.c
	$(CC) $(M_CFLAGS) -c -o $@ $<



all:
	gcc -o cpu_load.out main.c -O2 -Wall

monitor: $(OBJS)
	$(CC) -o $(MONITOR) $^ $(LDFLAGS)

clean:
	-rm *.o
	-rm -f *.out

run:
	make clean
	make all
	./cpu_load.out


