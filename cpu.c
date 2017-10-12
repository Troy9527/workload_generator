#include "cpu.h"

double 		cpu_load;
static char 	*cpu_buffer = NULL;
static FILE	*cpu_file;


static void read_cpu(unsigned long long* idle, unsigned long long* total);
static void exit_handler(void* none);

void* cpu_stat(void* none){
	unsigned long long idle[2];
	unsigned long long total[2];
	unsigned long long idle_diff, total_diff;
	
	/*registrate exit handler*/
	pthread_cleanup_push(exit_handler, NULL);

	/*allocate memory*/
	cpu_buffer = (char*)malloc(SIZE*sizeof(char));
	if(cpu_buffer == NULL)
		fprintf(stderr, "read_cpu: malloc failed\n");
	
	/*read stat*/
	while(1){
		/*read cpu at time1*/
		read_cpu(&idle[0], &total[0]);

		usleep(500000);

		/*read cpu at time2*/
		read_cpu(&idle[1], &total[1]);
		
		idle_diff = idle[1] - idle[0];
		total_diff = total[1] - total[0];
		cpu_load = 1.0 - ((double)idle_diff/(double)total_diff);

	}

	/*pop out handler from stack*/
	pthread_cleanup_pop(0);
}

static void read_cpu(unsigned long long* idle, unsigned long long* total){
	unsigned long long u, n, s, i, io, irq, soft;

	memset(cpu_buffer, 0, SIZE);

	/*open file*/
	cpu_file = fopen("/proc/stat", "r");
	if(cpu_file == NULL)
		fprintf(stderr, "read_cpu: fopen failed\n");

	fgets(cpu_buffer, SIZE, cpu_file);
	sscanf(cpu_buffer, "cpu %llu %llu %llu %llu %llu %llu %llu", &u, &n, &s, &i, &io, &irq, &soft);
	*idle = i;
	*total = (u + n + s +i + io + irq + soft);
	
	fclose(cpu_file);

}

static void exit_handler(void* none){
	if(cpu_buffer != NULL)
		free(cpu_buffer);

	if(cpu_file != NULL)
		fclose(cpu_file);
}
