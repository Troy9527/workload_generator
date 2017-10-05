#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "sighandler.h"
#include "stat.h"

static void usage(){
	printf("Hello");
}


int main(int argc, char* argv[]){
	int cpu_count, i;

	/*parsing arguments*/
	if(argc == 1){
		usage();
		return 0;
	}
	else{
		sscanf(argv[1],"%lf", &load);
		load = load/100.0;
	}

	/*find CPU cores*/
	cpu_count = (int) sysconf(_SC_NPROCESSORS_ONLN);
	if(cpu_count < 1){
		fprintf(stderr, "Error: could not determine CPU cores count\n");
		return cpu_count;
	}
	printf("Find %d CPU cores\n", cpu_count);

	/*registrate handler*/
	signal(SIGUSR1, (__sighandler_t)sigusr_handler);
	signal(SIGUSR2, (__sighandler_t)exit_handler);

	/*cpu_count = 1;*/
	/*start generate workload*/
	pid = (int*)malloc(cpu_count*sizeof(int));	

	int ret = setpriority(PRIO_PROCESS, 0, -20);
	/*printf("ret: %d\n", ret);*/

	/*create child process*/
	for(i=0; i<cpu_count; i++){
		pid[i] = fork();
		
		/*child process*/
		if(pid[i] == 0){
			clock_gettime(CLOCK_REALTIME, &start);
			while(1){
			}
		}
	}

	signal(SIGTERM, (__sighandler_t)sigterm_handler);

	while(1){

		usleep(50000);
		for(i=0; i<cpu_count; i++){
			kill(pid[i], SIGUSR1);
		}
		cpu_stat();	
	}

	free(pid);
	return 0;
}
