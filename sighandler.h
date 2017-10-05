#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "timer.h"


struct 	timespec start;
int 	*pid;
double 	load, mem_load, io_load;

/*signal handler*/
void sigusr_handler(void){
	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &end);
	/*printf("%d %d %ld %ld\n", start.tv_sec, end.tv_sec, start.tv_nsec, end.tv_nsec);*/
	double work = diff_in_second(start,end);
	double idle = (work*(1.0-load))/load;
	/*printf("%lf %lf\n", work, idle);*/

	usleep((int)idle);	
	
	clock_gettime(CLOCK_REALTIME, &start);
}

void sigterm_handler(void){
	int i;
	for(i=0; i<(sizeof(pid)/sizeof(int)); i++){
		kill(pid[i], SIGUSR2);
	}
	
	exit(0);
}	

void exit_handler(void){
	printf("exit\n");
	exit(0);
}

