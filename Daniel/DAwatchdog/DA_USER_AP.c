/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog (simple pase)

**************************	Header & Macro	********************************/
#define _POSIX_SOURCE /*include expand library of signal.h (sigation)*/
#include <assert.h>
#include <stdio.h> /*fprintf, scanf*/
#include <stdlib.h> /*system */
#include <unistd.h> /*pid_t*/
#include <pthread.h>
#include <sys/types.h>
#include <signal.h> /*sigaction*/
#include <semaphore.h>	/*	sem_op	*/
#include <string.h>
#include <fcntl.h>/*O_CREAT for sem_op*/
#include <errno.h>


#include "aux_funcs.h"

pthread_t tid = 1; 


int main(int argc, char *argv[])
{
	int i = 0;
	size_t max_fails = 7;
	size_t signal_intervals = 2;

    printf("                \n[%zu]MainUser\n\n", (size_t)pthread_self());

			/*printf("\n USER APP IS RUNNING|\n");*/
	
	/*Start watchdoog*/
	MakeMeImurtal(argc, argv, signal_intervals, max_fails);
	
	while(i < 10)
	{
		printf("                \nUSER APP PROGRAM [pid:%d] \n\n", getpid());
		sleep(1);
	}
	
    pthread_join(tid, NULL);

	/*End watchdoog*/
	DNR();

	return (0);
}
