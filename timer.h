/*#include <sys/time.h>*/
/*#include <sys/resource.h>*/

/*struct rusage rusage;*/

/*double dtime(){*/
	/*double ret;*/
	/*while(1){}*/
	/*getrusage(RUSAGE_SELF, &rusage);*/
	/*ret = (double)(rusage.ru_utime.tv_sec) + (double)((rusage.ru_utime.tv_usec)*1.0e-06);*/
	/*return ret;*/
/*}*/

#ifndef __TIMER_H
#define __TIMER_H

#include <time.h>

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec*1000000 + diff.tv_nsec / 1000.0);
}

#endif
