#include "cpuload.h"

void cpuloadgen(void){
	/*registrate handler*/
	signal(SIGUSR2, (__sighandler_t)exit_handler);
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
	}

	free(pid);
}
