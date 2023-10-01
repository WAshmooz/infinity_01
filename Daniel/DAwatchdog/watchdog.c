/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog 

**************************	Header & Macro	********************************/
#define _POSIX_SOURCE /*include expand library of signal.h (sigation)*/
#include <assert.h>
#include <stdio.h> /*fprintf, scanf, snprintf*/
#include <stdlib.h> /*system */
#include <unistd.h> /*pid_t*/
#include <pthread.h>
#include <sys/types.h>
#include <signal.h> /*sigaction*/
#include <semaphore.h>	/*	sem_op	*/
#include <string.h>
#include <fcntl.h>/*O_CREAT for sem_op*/
#include <errno.h>

#include "scheduler.h"
#include "aux_funcs.h"
#include "common.h"	/*assert, wd_args_t, pid_t, assert, 
						exit, atoi, puts									*/



/****************************Function prototypes*******************************/
int *WatchdogThread(wd_args_t *wd_args_);

/******************************Global variables********************************/
extern pthread_t tid; 

static char buf_max_fails[10]; 
static char buf_signal_intervals[10];
/**********************************Functions***********************************/

/*User function to initialize the watchdog*/
int MakeMeImurtal(int argc_, char *argv_[], size_t signal_intervals,
                                                             size_t max_fails) 
{
    int status = 0;
    wd_args_t *wd_args = NULL;

    /*Block SIGUSR1/2 from the user app*/
    status = BlockSIGUSR12Handler();
    RETURN_IF_ERROR(0 == status, "BlockSIGUSR12Handler error", status);

    /*Alocate for struct*/
    wd_args = (wd_args_t *)malloc(sizeof(wd_args_t));
    RETURN_IF_ERROR(NULL != wd_args,
             "Failed to allocate memory for wd_args struct!\n", EXIT_FAILURE);

    /*	asserts */
    assert(signal_intervals);
    assert(max_fails);

    /*Int to ASCI to pass signal_intervals & max_fails in argv_list*/
    snprintf(buf_max_fails, sizeof(buf_max_fails), "%d", max_fails); 
    snprintf(buf_signal_intervals, sizeof(buf_signal_intervals),
                                                     "%d", signal_intervals); 

    /*Allocate memory for argv_list (+2 for NULL and program_name)*/ 
    wd_args->argv_list = (char **)(calloc(argc_ + 4, sizeof(char *)));
    RETURN_IF_ERROR(NULL != wd_args->argv_list,
                                         "Eror: allocate arg_list", ILRD_FALSE);

    /*Set wd_args new added arguments at first 3 sells*/
    wd_args->argv_list[0] = "./watchdog_main.out";
    wd_args->argv_list[1] = buf_max_fails;
    wd_args->argv_list[2] = buf_signal_intervals;

    /*jump 3 sells and copy all args of user app to arg_list*/
    memcpy(wd_args->argv_list + 3, argv_, argc_ * sizeof(char *));

    /*set the rest of the struct*/
    wd_args->max_fails = max_fails; 
    wd_args->signal_intervals = signal_intervals;
    wd_args->is_user_prog = IS_USER_PROG;
    wd_args->signal_pid = getppid();
    wd_args->sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    RETURN_IF_ERROR(SEM_FAILED != wd_args->sem, "sem_open error", ILRD_FALSE);

    /*Create the watchdog thread*/
    status = pthread_create(&tid, NULL, (void *(*)(void *))&WatchdogThread,
                                                                     wd_args);
    RETURN_IF_ERROR(0 == status, "Eror: creating thread", ILRD_FALSE); 

    
    sem_wait(wd_args->sem);
    RETURN_IF_ERROR(0 == sem_close(wd_args->sem),
                                             "sem_close error", ILRD_FALSE);

    DEBUG printf("MakeMeImurtal after wait\n");


    return 0;
}

int *WatchdogThread(wd_args_t *wd_args_)
{	
    pid_t pid;
    int i = 0;

    sigset_t signal_set = {0};
    char *args[] = {"./watchdog.out", NULL};
    int ststus_sigaction = 0; /* SUCCESS */
    struct sigaction handler = {0};	

    /*Block all signals from WD_Thread and Unblock SIGUSR1 and SIGUSR2 */
    ExitIfError(0 == BlockAllSignalsHandler(),
                        "Error: BlockAllSignalsHandler failed", ILRD_FALSE);
    ExitIfError(0 == UnBlockSIGUSR12Handler(),
                        "Error: UnBlockSIGUSR12Handler failed", ILRD_FALSE);

    /* Create signal handler for SIGUSR1 (set counter to 0)*/
    handler.sa_handler = &SignalCountHandle;
    ststus_sigaction = sigaction(SIGUSR1, &handler, NULL);
    ExitIfError(ILRD_SUCCESS == ststus_sigaction,
                                     "Error: sigaction1 failed", ILRD_FALSE); 

    DEBUG printf("Fork START\n");

    pid = fork(); 
    if (0 == pid)
    {
        DEBUG printWdArgs(wd_args_);

        execvp(wd_args_->argv_list[0], wd_args_->argv_list);
        DEBUG printf("WE ARE NOT SUPPOSED TO GET HERE\n");
        assert(1 == 0);
    }
    
    if (0 < pid)
    {
        wd_args_->signal_pid = pid; /*child pid*/
        wd_args_->is_user_prog = IS_USER_PROG;
        /*wait for feedback from the watchdog*/
        sleep(2);

        WDSchedulerManage(wd_args_);
    }


    return NULL;   
} 

int DoNotResuscitate(void)
{
	void *status = NULL;

	/*by sending a signal to itself, I can be sure that the created thread will
	receive it and process it, and the main thread will ignore it*/
	RETURN_IF_ERROR(-1 != kill(getpid(), SIGUSR2), "kill error", KILL_FAIL);
	
	RETURN_IF_ERROR(-1 != sem_unlink(SEM_NAME), "sem_unlink error", 
															SEMUNLINK_FAIL);
	
	ExitIfError(0 == pthread_join(tid, &status), "pthread_join error", 
																	JOIN_FAIL);
	
	return (size_t)status;
}


