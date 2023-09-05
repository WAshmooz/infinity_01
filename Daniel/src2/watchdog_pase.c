/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		03.09.23
****Version:			1
****Description:		Watchdog (simple pase)

**************************	Header & Macro	********************************/
#define _POSIX_SOURCE /*include expand library of signal.h (sigation)*/
#include <stdio.h> /*fprintf, scanf*/
#include <stdlib.h> /*system */
#include <unistd.h> /*pid_t*/
#include <pthread.h>
#include <sys/types.h>
#include <signal.h> /*sigaction*/

#define UNUSED(x) (void)(x) /*include aux_funcs*/

#include "scheduler.h"

typedef struct 
{
	char **arg_list;
	volatile size_t sec_bet_signals;
	volatile size_t f_count;
} my_args_t;
/***********************	Function Declerations	**************************/


/***************************    Main    **************************************/
static volatile int g_count = 1;

static void HandleSignal(int signum) 
{
    if (signum == SIGUSR1) 
    {
        g_count = 0;
    }
}

/*OpFunction1 - not using params*/
static int OpFunction1(void *params)
{
    /*send signal*/
    UNUSED(params);
    if (0 == g_count)
    {
        printf(" OpFunction1     g_count = 0\n");
        return 0;
    }
    
    printf(" OpFunction1     g_count = 1\n");
    return 0;
}

int *StartSchedulerThread()
{   
    /*create signal*/
    int ststus_sigaction = 0; /*SUCCESS*/
	struct sigaction handler = {0};	
    /*create sched*/
    scheduler_t *sched = SchedCreate();
	handler.sa_handler = &HandleSignal;
	
    ststus_sigaction = sigaction(SIGUSR1, &handler, NULL);
    /*ETURN_IF_ERROR(ILRD_SUCCESS == ststus_sigaction, "Eror: sigaction1 failed", ILRD_FALSE); */


    /*add task to sched*/
    /*Uid_t uid = Sched() --> check uid*/
    SchedAdd(sched, OpFunction1, NULL, 1);

    /*run sched*/
    SchedRun(sched);

    /*CleanUp*/
    SchedDestroy(sched);
    return NULL;   
} 

int main() 
{
    int status = 2;
    pthread_t tid; 
    void *thread_arg = NULL;

    status = pthread_create(&tid, NULL, (void *(*)(void *))&StartSchedulerThread, thread_arg);
    if (status != 0)
    {
        fprintf(stderr, "Error creating thread \n");
        exit(1);
    }

    pthread_join(tid, NULL);

    return 0;
}




