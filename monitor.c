#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>

#include "display.h"

/*#include "stat.h"*/

static void initial(void);

int main(void){

	int 		command;
	pthread_t	display_thread;

	initial();
	
	/*create thread to display stat*/
	pthread_create(&display_thread, NULL, display, NULL);

	 /*parsing input*/
	while(1){
		refresh();
		switch(command = getch()){
			case 81:
				pthread_cancel(display_thread);
				endwin();
				exit(1);
				break;
			case 113:
				pthread_cancel(display_thread);
				endwin();
				exit(1);
				break;
		}
		
	}
	

	endwin();
}


static void initial(void){	
	initscr();		/* initial virtual screen */
	keypad(stdscr, TRUE);	/* enable keyboard */
	move(2, 2);		/* move cursor to (2,2) */
	/*mvaddstr(3, 3, "hello world\n");*/
}
