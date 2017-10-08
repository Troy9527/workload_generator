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


struct 	timespec start;
int 	*pid;
double 	load, mem_load, io_load, io_max;
int 	cpu_count;
unsigned long long	size;
char	*temp, *io_buffer;
FILE	*file;

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

/*void sigusr_mem_handler(void){*/
	/*int stride = 4096;*/
	/*munlock(temp, pre_size);*/
	/*unsigned long long i, size = ((*share)/mem_count)*1024;*/
	
	/*temp = (char *)mremap(temp, pre_size, size, MREMAP_MAYMOVE);	*/
	/*temp = (char *)realloc(temp, size);*/
	/*if(temp == NULL) fprintf(stderr, "realloc fail %llu\n", size);*/
	/*for(i=0; i<size; i+=stride) temp[i] = 'z';*/
	
	/*mlock(temp, size);*/
	/*pre_size = size;*/
	/*pause();*/
/*}*/


void sigterm_handler(void){
	int i;
	for(i=0; i< cpu_count; i++){
		kill(pid[i], SIGUSR2);
	}
	
	free(pid);
	exit(0);
}	

void sigterm_io_handler(void){
	kill(pid[0], SIGUSR2);
	
	free(pid);
	exit(0);
}	

void sigterm_mem_handler(void){
	/*int i;*/
	/*for(i=0; i<mem_count; i++){*/
		/*kill(pid[i], SIGUSR2);*/
	/*}*/

	free(temp);
	exit(0);
}	

void exit_handler(void){
	printf("exit\n");
	exit(0);
}

void exit_io_handler(void){
	printf("exit\n");
	fclose(file);
	free(io_buffer);
	exit(0);
}

/*void exit_mem_handler(void){*/
	/*printf("exit\n");*/
	/*shmdt(share);*/
	/*munlock(temp, sizeof(temp));*/
	/*free(temp);*/
	/*exit(0);*/
/*}*/

#endif
