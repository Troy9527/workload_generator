#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#include "timer.h"


struct 	timespec start;
int 	*pid;
double 	load, mem_load, io_load;
int 	shm_id, cpu_count, mem_count;
unsigned long long	*share, pre_size;
char	*temp;

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

void sigusr_mem_handler(void){
	int stride = 4096;
	munlock(temp, pre_size);
	unsigned long long i, size = ((*share)/mem_count)*1024;
	
	temp = realloc(temp, size);	
	if(temp == NULL) fprintf(stderr, "realloc fail %llu\n", size);
	for(i=0; i<size; i+=stride) temp[i] = 'z';
	
	mlock(temp, size);
	pre_size = size;
	pause();
}


void sigterm_handler(void){
	int i;
	for(i=0; i< cpu_count; i++){
		kill(pid[i], SIGUSR2);
	}
	
	free(pid);
	exit(0);
}	

void sigterm_mem_handler(void){
	int i;
	for(i=0; i<mem_count; i++){
		kill(pid[i], SIGUSR2);
	}

	shmdt(share);
	free(pid);
	exit(0);
}	

void exit_handler(void){
	printf("exit\n");
	exit(0);
}

void exit_mem_handler(void){
	printf("exit\n");
	shmdt(share);
	munlock(temp, sizeof(temp));
	free(temp);
	exit(0);
}

