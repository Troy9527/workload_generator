#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>


#include "cpu.h"
#include "iodisk.h"

/*extern double cpu_load;*/

void* display(void* none);


#endif
