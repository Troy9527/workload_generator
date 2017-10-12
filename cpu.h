#ifndef __CPU_H
#define __CPU_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>

#define SIZE 3000

extern double cpu_load;

void* cpu_stat(void* none);

#endif
