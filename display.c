#include "display.h"

void* display(void* none){
	pthread_t read_threads[3];

	/*registrate exit handler*/
	pthread_cleanup_push(handler, NULL);



	pthread_cleanup_pop(0);
	return NULL;
}


void exit_handler(void* none){
	printw("ggininer\n");
}
