#include "cpu.h"

void* cpu_stat(void* none){


	/*registrate exit handler*/
	pthread_cleanup_push(exit_handler, NULL);

	/*read stst*/
	while(1){
		
	}

	/*pop out handler from stack*/
	pthread_cleanup_pop(0);
}


void exit_handler(void* none){
	
}
