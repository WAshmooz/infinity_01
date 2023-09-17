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

static void SignalCountHandle(int signum);

volatile int g_count = 1;


int main(int argc, char *argv[])
{
    int i = 0;

    int ststus_sigaction = 0; /*SUCCESS*/
    struct sigaction handler = {0};	    /*Create signal handler for SIGUSR1*/

    printf("                                STARTING watchdog_process\n");
	handler.sa_handler = &SignalCountHandle;
    ststus_sigaction = sigaction(SIGUSR1, &handler, NULL);
    ExitIfError(ILRD_SUCCESS == ststus_sigaction, "Eror: sigaction1 failed", ILRD_FALSE); 

    for ( i = 0; i < 10; i++)
    {
        printf("                                Watchdog_Process sending signal\n");
        kill(getppid(), SIGUSR1);
        sleep(1);
        sleep(1);

    }
    

    return 0;
}

static void SignalCountHandle(int signum) 
{
    printf("                [%zu]handlerFun_of_process\n", (size_t)pthread_self());

    if (signum == SIGUSR1) 
    {
        g_count = 0;
    }
}