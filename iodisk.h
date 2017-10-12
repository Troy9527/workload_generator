#ifndef __IODISK_H
#define __IODISK_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>

#define NAME_SIZE 16

struct	iodisk{
	char 	name[NAME_SIZE];
	int 	major;
	int 	minor;
	double	write_rate;
	double	read_rate;
	double	total_rate;
	unsigned long long write_sec;
	unsigned long long read_sec;
	

	struct iodisk *pre;
	struct iodisk *next;
};

struct iolist{
	struct iodisk *head;
	struct iodisk sentinel;
};


extern struct iolist dlist;
extern double io_load;

void* io_stat(void* none);

#endif
