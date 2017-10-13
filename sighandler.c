#include "sighandler.h"

struct timespec 	start;
int 			*pid, *io_pid;
double 			load, mem_load, io_load, io_max;
int 			cpu_count, do_cpu=0, do_mem=0, do_io=0;
unsigned long long	mem_size;
char			*mem_buffer, *io_buffer;
FILE			*file;


void sigusr_handler(void){
	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &end);

	double work = diff_in_second(start,end);
	double idle = (work*(1.0-load))/load;
	/*printf("%lf %lf\n", work, idle);*/

	usleep((int)idle);	
	
	clock_gettime(CLOCK_REALTIME, &start);
}


void sigterm_handler(void){

	if(do_cpu){
		int i;
		for(i=0; i< cpu_count; i++){
			kill(pid[i], SIGUSR2);
		}
		free(pid);
	}

	if(do_mem){
		munlock(mem_buffer, mem_size);
		free(mem_buffer);
	}

	if(do_io){
		kill(*io_pid, SIGUSR2);
		free(io_pid);
	}
	exit(0);
}	


/* exit handler */
void exit_handler(void){
	exit(0);
}

void exit_io_handler(void){
	unlink("/home/troy/tmp");
	fclose(file);
	free(io_buffer);
	exit(0);
}
