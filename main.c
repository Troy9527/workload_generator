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

void cpuloadgen(int cpu_count){	
	signal(SIGUSR1, (__sighandler_t)sigusr_handler);

	/*start generate workload*/
	pid = (int*)malloc(cpu_count*sizeof(int));	

	int i, ret = setpriority(PRIO_PROCESS, 0, -20);
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
}

int main(int argc, char* argv[]){
	int	cpu_count, i=1, do_cpu=0, do_mem=0, do_io=0;

	/*parsing arguments*/
	if(argc == 1){
		usage();
		return 0;
	}
	else{
		while(i < argc){
			switch(argv[i][0]){
				case 'c':
					sscanf(argv[i+1],"%lf", &load);
					load = load/100.0;
					i += 2;
					do_cpu = 1;
					break;
				case 'm':
					sscanf(argv[i+1],"%lf", &mem_load);
					mem_load = mem_load/100.0;
					i += 2;
					do_mem = 1;
					break;
				case 'i':
					sscanf(argv[i+1],"%lf", &io_load);
					io_load = io_load/100.0;
					i += 2;
					do_io = 1;
					break;

			}
		}
	}

	/*printf("%lf %lf %lf\n", load, mem_load, io_load);*/

	/*find CPU cores*/
	cpu_count = (int) sysconf(_SC_NPROCESSORS_ONLN);
	if(cpu_count < 1){
		fprintf(stderr, "Error: could not determine CPU cores count\n");
		return cpu_count;
	}
	printf("Find %d CPU cores\n", cpu_count);

	/*registrate exit handler*/
	signal(SIGUSR2, (__sighandler_t)exit_handler);

	/*generate load*/
	if(do_cpu){
		cpuloadgen(cpu_count);
	}	
	
	if(do_mem){
		
	}

	if(do_io){
		
	}

	return 0;
}
