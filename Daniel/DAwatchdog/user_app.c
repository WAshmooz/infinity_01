/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog (simple pase)

**************************	Header & Macro	********************************/
#include <stdio.h> /*fprintf, scanf*/
#include <unistd.h> /*pid_t*/
#include <pthread.h>

#include <signal.h> /*sigaction*/
#include <semaphore.h>	/*	sem_op	*/
#include <fcntl.h>/*O_CREAT for sem_op*/

pthread_t tid = 1;

int main(int argc, char *argv[])
{
	int i = 0;
	size_t max_fails = 7;
	size_t signal_intervals = 2;
	
	/*Start watchdoog*/
	MakeMeImurtal(argc, argv, signal_intervals, max_fails);
	
	/*flow of user code*/
	while(i < 10)
	{
		++i;
		printf("\nUSER APP PROGRAM [pid:%d] \n\n", getpid());
		sleep(3);
	}
	
	pthread_join(tid, NULL);
	
	/*End watchdoog*/
	DoNotResuscitate();

	return (0);
}
