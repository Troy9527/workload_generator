#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/mman.h>

#include "sighandler.h"
#include "stat.h"

static void usage(){
	printf("Hello");
}

void cpuloadgen(){
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
		cpu_stat();	
	}

	free(pid);
}

void memloadgen(){
	/*registrate handler*/
	signal(SIGUSR1, (__sighandler_t)sigusr_mem_handler);
	signal(SIGUSR2, (__sighandler_t)exit_mem_handler);

	/*start generate workload*/
	int	i, stride = 4096, mem_count = 10;
	pid = (int*)malloc(mem_count*sizeof(int));
	unsigned long long total, free_mem, use, j;
	mem_get(&total, &free_mem);
	use = (total*(mem_load-1)) + free_mem;  // kB

	printf("use: %llu\n", use);

	/*shared memory*/
	shm_id = shmget(0, sizeof(unsigned long long), 0777 | SHM_R | SHM_W | IPC_CREAT);

	for(i=0; i<mem_count; i++){
		pid[i] = fork();
		
		/*child process*/
		if(pid[i] == 0){
			int nonce;
			share = shmat(shm_id, NULL, 0);
			pre_size = (use/mem_count)*1024;
			temp = (char *)malloc(pre_size);
			/*memset(temp, 3, pre_size);*/
			
			/*access memory address*/
			if(temp == NULL) fprintf(stderr, "malloc fail %llu\n", pre_size);
			for(j=0; j<pre_size; j+=stride) temp[j] = 'z';
			printf("%llu\n", pre_size);
			mlock(temp, pre_size);

			pause();
		}
	}
	
	signal(SIGTERM, (__sighandler_t)sigterm_mem_handler);
	share = shmat(shm_id, NULL, 0);
	
	while(1){
		mem_get(&total, &free_mem);
		use = (total*(mem_load-1)) + free_mem;  // kB
		usleep(50000);
		/*usleep(5000000);*/
		
		for(i=0; i<mem_count; i++){
			kill(pid[i], SIGUSR1);
		}
		mem_stat();	
	}

	shmdt(share);
	free(pid);
}

int main(int argc, char* argv[]){
	int i=1, do_cpu=0, do_mem=0, do_io=0;

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

	/*generate load*/
	if(do_cpu){
		cpuloadgen();
	}	
	
	if(do_mem){
		memloadgen();
	}

	if(do_io){
		
	}

	return 0;
}
