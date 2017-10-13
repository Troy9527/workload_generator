#ifndef __SIGHANDLER_H
#define __SIGHANDLER_H

#define _GNU_SOURCE
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <linux/mman.h>

#include "timer.h"


extern struct timespec 	start;
extern int 		*pid, *io_pid;
extern double 		load, mem_load, io_load, io_max;
extern int 		cpu_count, do_cpu, do_mem, do_io;
extern unsigned long long	mem_size;
extern char		*mem_buffer, *io_buffer;
extern FILE		*file;


void sigusr_handler(void);

void sigterm_handler(void);

void exit_handler(void);
void exit_io_handler(void);

#endif
