#include "display.h"


static void exit_handler(void* none);
static void mem_stat(void);

static pthread_t 	read_threads[3];
static FILE *		mem_file;
static double		mem_load;
static unsigned long long mem_used, MemTotal;


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
		mvprintw(3, 3, "CPU: %.3lf%\n", cpu_load*100.0);
		mvprintw(5, 3, "mem: %.3lf%\n", mem_load*100.0);
		mvprintw(6, 8, "%llu kB/ %llu kB\n", mem_used, MemTotal);
		
		/* read io stat*/
		for(node = dlist.sentinel.pre, i=9 ; node && node != &dlist.sentinel ; i += 6, node = node->pre){
			mvprintw(i, 3,   "%s\n", node->name, node->total_rate);
			mvprintw(i+1, 3, "--------------------\n");
			mvprintw(i+2, 3, "read: %.2lfkB/s\n ", node->read_rate);
			mvprintw(i+3, 3, "write: %.2lfkB/s\n", node->write_rate);
			mvprintw(i+4, 3, "total: %.2lfkB/s\n", node->total_rate);
		}

		dick();

		refresh();
		usleep(1200000);
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

	unsigned long	MemFree, Buffers;
	unsigned long	Cached, Shmem, SReclaimable;	

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
	int cpu_length, mem_length, i;
	
	init_pair(0, COLOR_YELLOW, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_YELLOW);

	cpu_length = 56*cpu_load;
	mem_length = 56*mem_load;

	start_color();

	if(cpu_load < 0.3){
		mvprintw(52 - cpu_length, 180, "     *****     ");
		mvprintw(53 - cpu_length, 180, "   **     **   ");
		mvprintw(54 - cpu_length, 180, " **         ** ");
		mvprintw(55 - cpu_length, 180, "**           **");	
		mvprintw(56 - cpu_length, 180, "**           **");
		
		for(i = 0 ; i<cpu_length ; i++){
			/*attron(COLOR_PAIR(0));*/
			/*attroff(COLOR_PAIR(0));*/
			mvprintw(56-i, 180, "  **       **");
		}
	}
	else if(cpu_load <0.7){
		mvprintw(52 - cpu_length, 180, "     *********     ");
		mvprintw(53 - cpu_length, 180, "   **         **   ");
		mvprintw(54 - cpu_length, 180, " **             ** ");
		mvprintw(55 - cpu_length, 180, "**               **");	
		mvprintw(56 - cpu_length, 180, "**               **");
		
		for(i = 0 ; i<cpu_length ; i++){
			/*attron(COLOR_PAIR(0));*/
			/*attroff(COLOR_PAIR(0));*/
			mvprintw(56-i, 180, "  **           **");
		}

	}
	else{

		mvprintw(49 - cpu_length, 180, "  *     *  *    *  ");
		mvprintw(50 - cpu_length, 180, "   *    *  *   *   ");
		mvprintw(51 - cpu_length, 180, "     *  *  *  *    ");
		mvprintw(52 - cpu_length, 180, "     *********     ");
		mvprintw(53 - cpu_length, 180, "   **         **   ");
		mvprintw(54 - cpu_length, 180, " **             ** ");
		mvprintw(55 - cpu_length, 180, "**               **");	
		mvprintw(56 - cpu_length, 180, "**               **");
		
		cpu_length = 56*cpu_load;
		for(i = 0 ; i<cpu_length ; i++){
			/*attron(COLOR_PAIR(0));*/
			/*attroff(COLOR_PAIR(0));*/
			mvprintw(56-i, 180, "  **           **");
		}

	}

	mvprintw(52 - mem_length, 140, "     *****     ");
	mvprintw(53 - mem_length, 140, "   **     **   ");
	mvprintw(54 - mem_length, 140, " **         ** ");
	mvprintw(55 - mem_length, 140, "**           **");
	mvprintw(56 - mem_length, 140, "**           **");



	

	for(i = 0 ; i<mem_length ; i++){
		mvprintw(56-i, 140, "  **       **");
	}
}
