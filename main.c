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
	printf("usage: workload [-c cpu_usage] [-m mem_usage] [-i io_usage io_max_speed]\n"
	       "  options:\n"
	       "    -c cpu_usage\t\tstress the utilization of CPU to cpu_usage\n"
	       "    -m mem_usage\t\tstress the utilization of memory to mem_usage\n"
	       "    -i io_usage io_max_speed\tstress the utilization of disk io to io_usage according to io_max_speed\n"		
	       );
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
			switch(argv[i][1]){
				case 'c':
					if(argv[i+1] != NULL){
						sscanf(argv[i+1],"%lf", &load);
					}
					else{
						usage();
						return 0;
					}
					
					load = load/100.0;
					i += 2;
					do_cpu = 1;
					break;
				case 'm':
					if(argv[i+1] != NULL){
						sscanf(argv[i+1],"%lf", &mem_load);
					}
					else{
						usage();
						return 0;
					}

					mem_load = mem_load/100.0;
					i += 2;
					do_mem = 1;
					break;
				case 'i':
					if(argv[i+1] != NULL){
						sscanf(argv[i+1],"%lf", &io_load);
					}
					else{
						usage();
						return 0;
					}
					
					if(argv[i+2] != NULL){
						sscanf(argv[i+2],"%lf", &io_max);
					}
					else{
						usage();
						return 0;
					}

					
					io_load = io_load/100.0;
					i += 3;
					do_io = 1;
					break;
				default:
					usage();
					return 0;

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
