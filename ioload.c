#include "ioload.h"


void ioloadgen(){
	/*registrate handler*/
	signal(SIGUSR2, (__sighandler_t)exit_io_handler);
	/*signal(SIGUSR1, (__sighandler_t)sigusr_handler);*/

	/*start generate workload*/
	pid = (int*)malloc(1*sizeof(int));	

	int			io_time = 1;
	unsigned long long	bytes = io_time*io_max*io_load*1024;
	double			expect_kbs = io_max*io_load;
	struct timespec		end;
	int			i;
	printf("Expect kbs: %lfkB/s\nioloadgen: write %llu bytes in %d seconds\n", expect_kbs, bytes, io_time);

	/*create child process*/
	for(i=0; i<1; i++){
		pid[i] = fork();
		
		/*child process*/
		if(pid[i] == 0){
			while(1){
				file = fopen("/home/troy/tmp", "w");
				if(file == NULL) fprintf(stderr, "io: create temp file failed\n");
				io_buffer = (char*)malloc(bytes*sizeof(char));

				/*start write to disk*/
				clock_gettime(CLOCK_REALTIME, &start);
					write(fileno(file), io_buffer, bytes);
					sync();
				clock_gettime(CLOCK_REALTIME, &end);
				
				double 	write_time = diff_in_second(start, end);
				/*printf("write second: %lf\n", write_time);				*/
				
				/* check write time*/
				if(write_time > io_time*1000000){
					fprintf(stderr, "write time is too long\n");
					exit(0);
				}

				int	sleep_time = (io_time*1000000 - write_time);
				/*printf("sleep second: %d\n", sleep_time);				*/
				usleep(sleep_time);

				free(io_buffer);
				fclose(file);
			}
		}
	}

	signal(SIGTERM, (__sighandler_t)sigterm_io_handler);

	pause();
	free(pid);

}
