#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <string.h>

#include "timer.h"
#include "stat.h"

struct timespec start;
double load;

static void usage(){
	printf("Hello");
}

/*signal handler*/
void sigterm_handler(void){
	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &end);
	/*printf("%d %d %ld %ld\n", start.tv_sec, end.tv_sec, start.tv_nsec, end.tv_nsec);*/
	double work = diff_in_second(start,end);
	double idle = (work*(1.0-load))/load;
	printf("%lf %lf\n", work, idle);

	usleep((int)idle);	
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
	signal(SIGTERM, (__sighandler_t)sigterm_handler);

	/*cpu_count = 1;*/
	/*start generate workload*/
	int i, pid[cpu_count];

	/*while(1){*/
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

		usleep(50000);
		for(i=0; i<cpu_count; i++){
			kill(pid[i], SIGTERM);
		}
		
	/*}*/


}
