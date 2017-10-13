#include "iodisk.h"

double		io_load;
static FILE	*io_file;
static char	*io_buffer;
static int	buffer_size = 1000;
struct iolist dlist = {NULL, {{0}, -1, -1, 0, 0, 0, -1, -1, NULL, NULL}};

static void init_dlist(void);
static void exit_handler(void* none);
static void read_disk(int time);
static struct iodisk* search_disk(int major, int minor);
static void append_disk(int major, int minor, char* name, unsigned long long write_sec,unsigned long long read_sec);

void* io_stat(void* none){

	int sleep_time = 1000000;	

	/* set sentinel node in dlist */
	init_dlist();

	io_buffer = (char*)malloc(buffer_size*sizeof(char));
	/*name = (char*)malloc(NAME_SIZE*sizeof(char));*/



	pthread_cleanup_push(exit_handler, NULL);
	while(1){
		read_disk(sleep_time);

		usleep(sleep_time);
	}
	pthread_cleanup_pop(0);
}

void exit_handler(void* none){
	if(io_file != NULL)
		fclose(io_file);

	free(io_buffer);
	
	/*free list*/
	struct iodisk *node = dlist.sentinel.pre;
	do{
		node = node->pre;
		free(node->next);
	}while(node != &dlist.sentinel);
}

void init_dlist(void){
	dlist.head = &(dlist.sentinel);
	dlist.sentinel.next = dlist.head;
}


void read_disk(int time){
	int 	major, minor;
	char	name[NAME_SIZE];	
	unsigned long long	write_sec, read_sec;

	time /= 1000000;
	io_file = fopen("/proc/diskstats", "r");
	if(io_file == NULL)
		fprintf(stderr, "io_stat: fopen failed\n");


	while(fgets(io_buffer, buffer_size, io_file) != NULL){
		sscanf(io_buffer, "%d %d %s %*llu %*llu %llu %*llu %*llu %*llu %llu"
				, &major, &minor, name, &read_sec, &write_sec);
		
		/* find whole disk information */
		if(major == 8 && (minor % 16) ==0){
			struct iodisk *node = search_disk(major, minor);
			if(node == NULL){  /* not found, need append to disk */
				append_disk(major, minor, name, write_sec, read_sec);
				
				fprintf(stderr, "%p %p %p %p\n", &dlist.sentinel, dlist.sentinel.next, dlist.sentinel.next->next, dlist.sentinel.next->next->next);
			}
			else{		   /* calculate rate */			
				node->total_rate = (((write_sec - node->write_sec) 
					+ (read_sec - node->read_sec))/2.0)/(double)time;
				node->write_rate = ((write_sec - node->write_sec)/2.0)/(double)time;
				node->read_rate = ((read_sec - node->read_sec)/2.0)/(double)time;

				node->write_sec = write_sec;
				node->read_sec = read_sec;

			}
		}

		memset(io_buffer, 0, buffer_size);
		memset(name, 0, NAME_SIZE);
	}

	fclose(io_file);
}

static struct iodisk* search_disk(int major, int minor){
	struct iodisk *node;
	dlist.sentinel.major = major;
	dlist.sentinel.minor = minor;

	for(node = dlist.head ; node->major != major || node->minor != minor
			; node = node->next){
	}
	
	if(node != &dlist.sentinel)
		return node;

	return NULL;
}

static void append_disk(int major, int minor, char* name, unsigned long long write_sec,unsigned long long read_sec){
	
	struct iodisk *temp = (struct iodisk*)malloc(sizeof(struct iodisk));
	temp->major = major;
	temp->minor = minor;
	temp->write_sec = write_sec;
	temp->read_sec = read_sec;
	strncpy(temp->name, name, NAME_SIZE);

	dlist.head = temp;

	temp->pre = &dlist.sentinel;
	temp->next = dlist.sentinel.next;
	dlist.sentinel.next->pre = temp;
	dlist.sentinel.next = temp;

}

