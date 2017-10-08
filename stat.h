#ifndef __STAT_H
#define __STAT_H

#include <stdio.h>
#include <stdlib.h>
#define SIZE 3000

void cpu_stat(){
	FILE *proc;
	unsigned long long idle1 = 0, idle2 = 0, total1 = 0, total2 = 0;
	unsigned long long u, n, s, i, io, irq, soft;

	char *buffer = (char*)malloc(SIZE*sizeof(char));

	/*read stat at time(1)*/
	proc = fopen("/proc/stat", "r");
	fgets(buffer,1000,proc);
	/*printf("%s\n", buffer);*/
	sscanf(buffer, "cpu %llu %llu %llu %llu %llu %llu %llu", &u, &n, &s, &i, &io, &irq, &soft);
	idle1 = i;
	total1 = (u + n + s +i + io + irq + soft);
	fclose(proc);		

	usleep(500000);
	memset(buffer, 0, SIZE);

	/*read stat at time(1)*/
	proc = fopen("/proc/stat", "r");
	fgets(buffer,1000,proc);
	/*printf("%s\n", buffer);*/
	sscanf(buffer, "cpu %llu %llu %llu %llu %llu %llu %llu", &u, &n, &s, &i, &io, &irq, &soft);
	idle2 = i;
	total2 = (u + n + s +i + io + irq + soft);
	fclose(proc);		

	unsigned long long t1,t2;
	t1 = (idle2-idle1) > 0 ? idle2-idle1 : idle1-idle2;
	t2 = (total2-total1) > 0 ? total2-total1 : total1-total2;

	double load = 1.0 - ((double)t1/(double)t2);
	printf("cpu load: %lf\n", load);
	
	free(buffer);
}

void mem_stat(){
	FILE 			*proc;
	char 			*buffer = (char*)malloc(SIZE*sizeof(char));	
	unsigned long long 	total, free_mem;	
	proc = fopen("/proc/meminfo", "r");
	
	/*read data*/
	memset(buffer, 0, SIZE);
	fgets(buffer,1000,proc);
	/*printf("%s\n", buffer);*/

	sscanf(buffer, "MemTotal: %llu", &total);
	
	memset(buffer, 0, SIZE);
	fgets(buffer,1000,proc);
	/*printf("%s\n", buffer);*/

	sscanf(buffer, "MemFree: %llu", &free_mem);

	/*printf("%llu %llu\n", total, free_mem);*/
	printf("mem load: %lf\n", (double)(1.0 - ((double)free_mem/(double)total)));

	fclose(proc);
	free(buffer);
}

void mem_get(unsigned long long *total, unsigned long long *free_mem){
	FILE 			*proc;
	char 			*buffer = (char*)malloc(SIZE*sizeof(char));	
	proc = fopen("/proc/meminfo", "r");
	
	/*read data*/
	memset(buffer, 0, SIZE);
	fgets(buffer,1000,proc);

	sscanf(buffer, "MemTotal: %llu", total);
	
	memset(buffer, 0, SIZE);
	fgets(buffer,1000,proc);

	sscanf(buffer, "MemFree: %llu", free_mem);

	/*printf("%llu %llu\n", total, free_mem);*/

	fclose(proc);
	free(buffer);

}	

void io_stat(){
	FILE 	*proc = fopen("/proc/diskstats", "r");
	char 	*buffer = (char*)malloc(SIZE*sizeof(char));	
	char	*name = (char*)malloc(30*sizeof(char));
	unsigned long long 	read_sec1, write_sec1, read_sec2, write_sec2;
	double	read_diff, write_diff;
	int time = 5;

	/*sample at time (1)*/
	if(proc == NULL)
		fprintf(stderr, "open /proc/diskstats fialed\n");

	while(fgets(buffer, 200, proc) != NULL){
		/*read major&minor number*/
		sscanf(buffer, "%llu %llu", &read_sec1, &write_sec1);
		/*printf("%s", buffer);*/

		if(read_sec1 == 8 && write_sec1 == 0){
			/*sscanf(buffer ,"%*[0-9] %*[0-9] %*[^0-9] %d", &a);*/
			sscanf(buffer ,"%*llu %*llu %s %*llu %*llu %llu %*llu %*llu %*llu %llu", name, &read_sec1, &write_sec1);
			/*printf("%s ", name);*/
			/*printf("%llu %llu\n", read_sec1, write_sec1);*/
			break;
		}
	}
	memset(buffer, 0, SIZE);
	memset(name, 0, 30);
	fclose(proc);

	sleep(time);
	printf("time2\n");

	/*sample at time (1)*/
	proc = fopen("/proc/diskstats", "r");
	if(proc == NULL)
		fprintf(stderr, "open /proc/diskstats fialed\n");

	while(fgets(buffer, 200, proc) != NULL){
		sscanf(buffer, "%llu %llu", &read_sec2, &write_sec2);
		/*printf("%s", buffer);*/

		if(read_sec2 == 8 && write_sec2 == 0){
			sscanf(buffer ,"%*llu %*llu %s %*llu %*llu %llu %*llu %*llu %*llu %llu", name, &read_sec2, &write_sec2);
			/*printf("%s\t", name);*/
			/*printf("%llu %llu\n", read_sec2, write_sec2);*/
			break;
		}
	}
	memset(buffer, 0, SIZE);
	memset(name, 0, 30);
	fclose(proc);

	write_diff = ((write_sec2 - write_sec1)/2.0)/(double)time;
	read_diff = ((read_sec2 - read_sec1)/2.0)/(double)time;

	printf("read: %lfkB/s\nwrite: %lfkB/s\n", read_diff, write_diff);


	free(buffer);
	free(name);
}

#endif
