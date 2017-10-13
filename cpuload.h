#ifndef __CPULOAD_H
#define __CPULOAD_H

#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "sighandler.h"

void cpuloadgen(void);

#endif
