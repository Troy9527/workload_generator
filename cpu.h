#ifndef __CPU_H
#define __CPU_H
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>

double cpu_load;

void* cpu_stat(void* none);

void exit_handler(void* none);


#endif
