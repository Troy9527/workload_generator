#include "ioload.h"


void ioloadgen(){
	/*registrate handler*/
	/*signal(SIGUSR1, (__sighandler_t)sigusr_handler);*/

	/*start generate workload*/
	io_pid = (int*)malloc(1*sizeof(int));	

	int			io_time = 1;
	unsigned long long	bytes = io_time*io_max*io_load*1024;
	double			expect_kbs = io_max*io_load;
	struct timespec		end;
	int			i;

	/* check the path to tempfile */	
	if(strlen(io_tempfile) == 0){
		sprintf(io_tempfile, "tempfile");
	}

	printf("ioloadgen: write to tempfile at %s\n", io_tempfile);
	
	printf("ioloadgen: write %llu bytes in %d seconds\n", expect_kbs, bytes, io_time);


	/*create child process*/
	for(i=0; i<1; i++){
		*io_pid = fork();
		/*child process*/
		if(*io_pid == 0){
			signal(SIGUSR2, (__sighandler_t)exit_io_handler);
			while(1){
				file = fopen(io_tempfile, "w");
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
					fprintf(stderr, "writing time is too long\n");
					/*exit(0);*/
					free(io_buffer);
					fclose(file);
					continue;
				}

				int	sleep_time = (io_time*1000000 - write_time);
				/*printf("sleep second: %d\n", sleep_time);				*/
				usleep(sleep_time);

				free(io_buffer);
				fclose(file);
			}
		}
	}

	/*signal(SIGTERM, (__sighandler_t)sigterm_io_handler);*/

}
