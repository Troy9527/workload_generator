#include "memload.h"

static void mem_get(unsigned long long *total, unsigned long long *free_mem);

void memloadgen(){

	/*start generate workload*/
	int stride = 4096;
	unsigned long long total, free_mem, use, j;
	
	/*get memory information*/
	mem_get(&total, &free_mem);
	use = (total*mem_load);  // kB

	/*signal(SIGTERM, (__sighandler_t)sigterm_mem_handler);*/

	mem_size = use*1024;
	mem_buffer = (char *)malloc(mem_size);

	/*access memory address*/
	if(mem_buffer == NULL) 
		fprintf(stderr, "malloc fail %llu\n", mem_size);
	
	for(j=0 ; j < mem_size; j+=stride) 
		mem_buffer[j] = 'z';
	
	printf("memloadgen: %llu bytes\n", mem_size);
	mlock(mem_buffer, mem_size);

}

static void mem_get(unsigned long long *total, unsigned long long *free_mem){
	FILE 			*proc;
	char 			*buffer = (char*)malloc(1000*sizeof(char));	
	proc = fopen("/proc/meminfo", "r");
	
	/*read data*/
	memset(buffer, 0, 1000);
	fgets(buffer,1000,proc);

	sscanf(buffer, "MemTotal: %llu", total);
	
	memset(buffer, 0, 1000);
	fgets(buffer,1000,proc);

	sscanf(buffer, "MemFree: %llu", free_mem);

	/*printf("%llu %llu\n", total, free_mem);*/

	fclose(proc);
	free(buffer);

}
