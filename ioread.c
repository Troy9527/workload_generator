#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define LEN 30

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


int main(void){
	int			i ,pid, major, minor, input_major = 8, input_minor = 0, time = 1, records;
	FILE			*proc;
	struct 	timespec 	start, end;
	char			*buffer, name[20];
	unsigned long long	pre_write_bytes = 0, write_bytes;
	double			dtime;
	double			list[LEN];

	buffer = (char *)malloc(1000*sizeof(char));
	
	pid = fork();
	
	/* run dd command */
	if(pid < 0){
		fprintf(stderr, "fork failed\n");
	}
	else if(pid == 0){
		char *args[] = {"/usr/bin/dd", "if=/dev/zero", "of=temp", "conv=fdatasync", 0};
		int ret = execvp("/usr/bin/dd", args);
		if(ret < 0) fprintf(stderr, "execvp failed\n");
	}


	clock_gettime(CLOCK_REALTIME, &start);

	while(1){
		
		/* timer */
		clock_gettime(CLOCK_REALTIME, &end);
		dtime = diff_in_second(start, end);
		if(dtime > 60){
			free(buffer);
			kill(pid, SIGTERM);
			unlink("temp");
			return 0;
		}

		proc = fopen("/proc/diskstats", "r");
		if(proc == NULL) fprintf(stderr, "open proc file failed,  %s\n", (char *)strerror(errno));
		while(fgets(buffer, 1000, proc) != NULL){
				sscanf(buffer, "%d %d %s %*llu %*llu %*llu %*llu %*llu %*llu %llu", &major, &minor, name, &write_bytes);

				if(major == input_major && minor == input_minor){
					if(pre_write_bytes == 0){
						pre_write_bytes = write_bytes;
						break;
					}


					double rate = ((double)(write_bytes - pre_write_bytes)/(double)time)/(double)(1024*2);
					pre_write_bytes = write_bytes;
					printf("%g kB/s\n", rate);
					
					/*break;*/

				}
		}

		usleep(time*1000000);



		fclose(proc);
	}
	free(buffer);
}	


