/*******************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog (simple pase)

**************************	Header & Macro	***********************************/

#define _POSIX_C_SOURCE /*include expand library of signal.h (sigation)*/
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
#include "aux_funcs.h"
#include "watchdog.h"
#include "common.h"	/*wd_args_t, pid_t, assert, 
						exit, atoi, puts									*/
/******************************Global variables********************************/
static volatile int g_fail_counter;
static volatile int g_is_not_resucitate;
static volatile int g_is_child_ready;

/**********************************Functions***********************************/
int main(int argc_, char *argv_[])
{
    int max_fail = 0, signal_intervals = 0;
    int status = 0; 
    struct sigaction handler = {0}; /*Create signal handler for SIGUSR1*/

    wd_args_t wd_args; /*Create a wd_args_t structure*/
    DEBUG printf("STARTING watchdog_process1\n");

    UNUSED(argc_);

    /*Block all signals*/
    status = BlockAllSignalsHandler();
    ExitIfError(0 == status, "SignalMaskHandler2 error", ILRD_FALSE);

    /* Unblock SIGUSR1 and SIGUSR2 */
    ExitIfError(0 == UnBlockSIGUSR12Handler(),
                            "Error: UnBlockSIGUSR12Handler failed", ILRD_FALSE);


    handler.sa_handler = &SignalCountHandle;
    status = sigaction(SIGUSR1, &handler, NULL);
    ExitIfError(ILRD_SUCCESS == status, "Eror: sigaction1 failed", ILRD_FALSE); 

	/*convert argc to numbers*/
	max_fail = atoi(argv_[1]);
	signal_intervals = atoi(argv_[2]);

    wd_args.argv_list = argv_;
    wd_args.max_fails = max_fail;
    wd_args.signal_intervals = signal_intervals;
    wd_args.is_user_prog = IS_WD_PROG;
    wd_args.signal_pid = getppid();

    DEBUG printWdArgs(&wd_args); 

    /*Open sem for wd_thread -> continue execution of user app*/
    wd_args.sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    /*if (wd_args.sem == SEM_FAILED) 
	{
		RETURN_IF_ERROR(-1 != kill(getppid(), SIGKILL),
                                                     "kill error", KILL_FAIL);

		return SEMOPEN_FAIL;
	}*/

    /*Sem post + Sem close*/
	RETURN_IF_ERROR(-1 != sem_post(wd_args.sem),
                                             "sem_post error", SEMPOST_FAIL);
	RETURN_IF_ERROR(-1 != sem_close(wd_args.sem),
                                             "sem_close error", SEMCLOSE_FAIL);

    WDSchedulerManage(&wd_args);

    DEBUG printf("STARTING watchdog_process1 end\n");

    return 0;
}

