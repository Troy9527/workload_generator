#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <sys/mman.h>

#include "cpuload.h"
#include "memload.h"
#include "ioload.h"
#include "sighandler.h"
/*#include "stat.h"*/


static void usage(){
	printf("Hello");
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
