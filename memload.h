#ifndef __MEMLOAD_H
#define __MEMLOAD_H

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/mman.h>

#include "sighandler.h"

void memloadgen(void);


#endif
