#include "display.h"


static void exit_handler(void* none);
static void mem_stat(void);
static pthread_t 	read_threads[3];
static FILE *		mem_file;
static double		mem_load;

static void dick(void);

void* display(void* none){

	int i;
	struct iodisk *node;
	
	pthread_create(&read_threads[0], NULL, cpu_stat, NULL);
	pthread_create(&read_threads[1], NULL, io_stat, NULL);
	
	/*registrate exit handler*/
	pthread_cleanup_push(exit_handler, NULL);
	
	while(1){
		clear();
		mem_stat();
		mvprintw(3, 3, "CPU: %lf%\n", cpu_load*100.0);
		mvprintw(4, 3, "mem: %lf%\n", mem_load*100.0);
		
		/* read io stat*/
		for(node = dlist.sentinel.pre, i=5 ; node && node != &dlist.sentinel ; i++, node = node->pre){
			mvprintw(i, 3, "%s: %lfkB/s\n", node->name, node->total_rate);
		}

		dick();

		refresh();
		usleep(1000000);
	}
	pthread_cleanup_pop(0);
	return NULL;
}


static void exit_handler(void* none){
	/*exit read threads*/
	pthread_cancel(read_threads[0]);
	pthread_cancel(read_threads[1]);
}

static void mem_stat(void){

	unsigned long	MemTotal, MemFree, Buffers;
	unsigned long	Cached, Shmem, SReclaimable, mem_used;	

	mem_file = fopen("/proc/meminfo", "r");
	if(mem_file == NULL)
		fprintf(stderr, "mem_stat: fopen failed\n");

	fscanf(mem_file, "%*[^0-9] %lu"		/* MemTotal */
			 "%*[^0-9] %lu"		/* MemFree */
			 "%*[^0-9] %*lu"		
			 "%*[^0-9] %lu"		/* Buffers */
			 "%*[^0-9] %lu"		/* Cached */
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %lu"		/* Shmem */
			 "%*[^0-9] %*lu"
			 "%*[^0-9] %lu" 	/* SReclaimable */
			, &MemTotal, &MemFree, &Buffers, &Cached, &Shmem, &SReclaimable);

	mem_used = MemTotal - MemFree - Buffers - Cached - SReclaimable + Shmem;
	
	mem_load = (double)mem_used/(double)MemTotal;

	fclose(mem_file);
}

static void dick(void){
	int col, row, length, i;
	getmaxyx(stdscr, row, col);
	
	init_pair(0, COLOR_RED, COLOR_YELLOW);
	init_pair(1, COLOR_RED, COLOR_YELLOW);

	length = 56*cpu_load;

	mvprintw(52 - length, 190, "     *****     ");
	mvprintw(53 - length, 190, "   **     **   ");
	mvprintw(54 - length, 190, " **         ** ");
	mvprintw(55 - length, 190, "**           **");
	mvprintw(56 - length, 190, "**           **");
	
	for(i = 0 ; i<length ; i++){
	mvprintw(56-i, 190, "  **       **");
	}
}
