#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

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

	/*start generate workload*/
	int	i, stride = 4096;
	unsigned long long total, free_mem, use, j;
	mem_get(&total, &free_mem);
	use = (total*mem_load);  // kB

	printf("use: %llu\n", use);

	signal(SIGTERM, (__sighandler_t)sigterm_mem_handler);

	size = use*1024;
	temp = (char *)malloc(size);

	/*access memory address*/
	if(temp == NULL) fprintf(stderr, "malloc fail %llu\n", size);
	for(j=0; j<size; j+=stride) temp[j] = 'z';
	printf("%llu\n", size);
	mlock(temp, size);

	
	while(1){
		usleep(50000);
		/*mem_stat();	*/
	}

}

void ioloadgen(){
	/*registrate handler*/
	signal(SIGUSR2, (__sighandler_t)exit_io_handler);
	/*signal(SIGUSR1, (__sighandler_t)sigusr_handler);*/

	/*start generate workload*/
	pid = (int*)malloc(1*sizeof(int));	

	int			io_time = 1;
	unsigned long long	bytes = io_time*io_max*io_load*1024;
	double			expect_kbs = io_max*io_load;
	struct timespec		end;
	int			i;
	printf("Exprct kbs: %lfkB/s\nioloadgen: write %llu bytes in %d seconds\n", expect_kbs, bytes, io_time);

	/*create child process*/
	for(i=0; i<1; i++){
		pid[i] = fork();
		
		/*child process*/
		if(pid[i] == 0){
			while(1){
				file = fopen("/home/troy/tmp", "w");
				if(file == NULL) fprintf(stderr, "io: create temp file failed\n");
				io_buffer = (char*)malloc(bytes*sizeof(char));

				/*start write to disk*/
				clock_gettime(CLOCK_REALTIME, &start);
					write(fileno(file), io_buffer, bytes);
					sync();
				clock_gettime(CLOCK_REALTIME, &end);
				double 	write_time = diff_in_second(start, end);
				printf("write second: %lf\n", write_time);				
				if(write_time > io_time*1000000){
					fprintf(stderr, "write time is too long\n");
					exit(0);
				}

				int	sleep_time = (io_time*1000000 - write_time);
				printf("sleep second: %d\n", sleep_time);				
				usleep(sleep_time);

				free(io_buffer);
				fclose(file);
			}
		}
	}

	signal(SIGTERM, (__sighandler_t)sigterm_io_handler);

	while(1){
		io_stat();	
	}

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
					sscanf(argv[i+2],"%lf", &io_max);
					i += 3;
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
		ioloadgen();		
	}

	return 0;
}
