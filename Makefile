MONITOR = monitor.out
WORKLOAD = workload.out

CC = gcc

CFLAGS = \
	   -Wall -O2 -pthread -lncurses -Wno-format

LDFLAGS = \
	  -lm -pthread -lncurses
M_OBJS := \
	display.o \
	cpu.o \
	iodisk.o \
	monitor.o

OBJS := \
	main.o \
	sighandler.o \
	cpuload.o \
	memload.o \
	ioload.o \
	timer.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


load: $(OBJS)
	$(CC) -o $(WORKLOAD) $^ $(LDFLAGS)


monitor: $(M_OBJS)
	$(CC) -o $(MONITOR) $^ $(LDFLAGS)

clean:
	-rm *.o
	-rm -f *.out

run_monitor:
	make clean
	make monitor
	./monitor.out

run_load:
	make clean
	make load
	./workload.out -c 50 -m 50 -i 50 153600

ioread:
	gcc -o ioread.out ioread.c
