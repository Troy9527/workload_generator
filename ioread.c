#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

void delete_file(){
	int pid;
	pid = fork();
	if(pid == 0){
		char *buf[] = {"rm", "-f", "/home/troy/tmp", 0};
		execvp("rm", buf);
	}
}	

int main(void){
	int	i,pid;
	FILE	*proc;
	struct 	timespec start, end;
	char	*buffer;
	unsigned long long	bytes;
	double	dtime;

	buffer = (char *)malloc(100*sizeof(char));
	
	for(i = 0; i<1; i++){
		pid = fork();
		
		memset(buffer, 0, 100);
		sprintf(buffer,"/proc/%d/io", pid);
		/*buffer[strlen(buffer)] = '\0';*/
		clock_gettime(CLOCK_REALTIME, &start);
		

		if(pid < 0){
			fprintf(stderr, "fork failed\n");
		}
		else if(pid == 0){
			char *args[] = {"/usr/bin/dd", "if=/dev/zero", "of=/home/troy/tmp", "conv=fdatasync", 0};
			int ret = execvp("/usr/bin/dd", args);
			if(ret < 0) fprintf(stderr, "execvp failed\n");
		}
		
		sleep(1);

		clock_gettime(CLOCK_REALTIME, &end);
		dtime = diff_in_second(start, end);

		proc = fopen(buffer, "r");
		if(proc == NULL) fprintf(stderr, "open proc file failed,  %s\n", (char *)strerror(errno));

		fgets(buffer, 100, proc);
		fgets(buffer, 100, proc);
		fgets(buffer, 100, proc);
		fgets(buffer, 100, proc);
		fgets(buffer, 100, proc);

		memset(buffer, 0, 100);
		fgets(buffer, 100, proc);

		sscanf(buffer, "write_bytes: %llu", &bytes);
		double result = ((((double)bytes/dtime)/(double)1024.0)/(double)1024.0);	
		/*printf("time: %lf\nbytes: %llu\n", dtime, bytes);*/
		printf("write speed: %lfMb/s\n", result);

		kill(pid, SIGTERM);
		delete_file();
		fclose(proc);
	}

	free(buffer);
}	

