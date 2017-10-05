#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "timer.h"
#include "stat.h"

struct timespec start;
double load;
int *pid;

static void usage(){
	printf("Hello");
}

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

int main(int argc, char* argv[]){
	int cpu_count;

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
	int i;
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
