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
	FILE *proc;
	proc = fopen("/proc/stat", "r");
		
}

void io_stat(){
	
}
