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
#include <semaphore.h>	/*sem_op*/
#include <string.h>
#include <fcntl.h>/*O_CREAT for sem_op*/
#include <errno.h>

#include "scheduler.h"
#include "common.h"	/* WDHandler, assert, wd_args_t, pid_t, assert, 
						exit, atoi, puts									*/
#include "aux_funcs.h"

static void SignalCountHandle(int signum);
int WDSchedulerManage(void);
int StamScheduler(void *params);
void printWdArgs(const wd_args_t *args);

volatile int g_count = 1;



int main(int argc, char *argv[])
{
    
    int i = 0;

    int ststus_sigaction = 0; /*SUCCESS*/
    struct sigaction handler = {0};	    /*Create signal handler for SIGUSR1*/
        

    wd_args_t wd_args; /*Create a wd_args_t structure*/
    wd_args.argv_list = argv;
    printf("                                STARTING watchdog_process1\n");

    printWdArgs(&wd_args); /*Pass the address of wd_args to printWdArgs*/

	handler.sa_handler = &SignalCountHandle;
    ststus_sigaction = sigaction(SIGUSR1, &handler, NULL);
    ExitIfError(ILRD_SUCCESS == ststus_sigaction, "Eror: sigaction1 failed", ILRD_FALSE); 

    WDSchedulerManage();

    for ( i = 0; i < 10; i++)
    {
        /*sleep(3);*/
        printf("                                Watchdog_Process sending signal\n");
        kill(getppid(), SIGUSR1);
        sleep(1);

    }
    
    printf("                                STARTING watchdog_process1 end\n");

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

int StamScheduler(void *params)
{
    UNUSED(params);

    printf("..\n");
    return 0;
}


int WDSchedulerManage(void)
{
    int ret_status = 0;
	
	scheduler_t *wd_scheduler = NULL;
	
    printf("\n\nWDSchedulerManage START1\n\n");

	/*Create scheduler*/
	wd_scheduler = SchedCreate();
	ExitIfError(NULL != wd_scheduler, "Failed to create a WatchDog Scheduler!\n", -1);
	
    /*SchedAdd(wd_scheduler, SendSignal, NULL, 1);*/
	/*ExitIfError(UID_BAD == SchedAdd(wd_scheduler, SendSignal, NULL, 1), "Failed to SchedAdd!\n", -1);*/
	SchedAdd(wd_scheduler, StamScheduler, NULL, 1);

	ret_status = SchedRun(wd_scheduler);
	
	SchedDestroy(wd_scheduler);
	
	/*	return if scheduler has successfully finished */
	return (ILRD_SUCCESS == ret_status ? ILRD_SUCCESS : ILRD_FALSE);
}




void printWdArgs(const wd_args_t *args) 
{
    int i = 0;
    printf("wd_process_args:\n");
    printf("  argv_list:\n");

    while (NULL != args->argv_list[i]) 
    {
        printf("    argv_list[%d]: %s\n", i, args->argv_list[i]);
        ++i;
    }
}

