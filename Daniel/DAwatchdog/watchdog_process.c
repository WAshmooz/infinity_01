/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog (simple pase)

**************************	Header & Macro	********************************/
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
#include "common.h"	/* WDHandler, assert, wd_args_t, pid_t, assert, 
						exit, atoi, puts									*/
#include "aux_funcs.h"

#define SEM_NAME "/wd_semaphore"

int BlockAllSignalsHandler(void); /*Block all signals*/
int UnBlockSignalHandler(void); /*UnBlock SIGUSR1, SIGUSR2*/
static void SignalCountHandle(int signum);
int WDSchedulerManage(wd_args_t *wd_args_);
int StamScheduler(void *params);
void printWdArgs(const wd_args_t *args);

/******************************Global variables********************************/

volatile int g_fail_counter;
volatile int g_is_not_resucitate;
volatile int g_is_child_ready;
/**********************************Functions***********************************/

int main(int argc_, char *argv_[])
{
    int i = 0, max_fail = 0, signal_intervals = 0;
    int status = 0; 
    struct sigaction handler = {0};	    /*Create signal handler for SIGUSR1*/
        

    wd_args_t wd_args; /*Create a wd_args_t structure*/
    printf("                                STARTING watchdog_process1\n");

    /*Block all signals*/
    status = BlockAllSignalsHandler();
    ExitIfError(0 == status, "SignalMaskHandler error", status);

    /* Unblock SIGUSR1 and SIGUSR2 */
    ExitIfError(0 != UnBlockSignalHandler(), "Error: sigaction1 failed", ILRD_FALSE);


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

    printWdArgs(&wd_args); 

    /*Open sem for wd_thread -> continue execution of user app*/
    wd_args.sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    /*if (wd_args.sem == SEM_FAILED) 
	{
		RETURN_IF_ERROR(-1 != kill(getppid(), SIGKILL), "kill error", KILL_FAIL);

		return SEMOPEN_FAIL;
	}*/

    /*Sem post + Sem close*/
	RETURN_IF_ERROR(-1 != sem_post(wd_args.sem), "sem_post error", SEMPOST_FAIL);
	RETURN_IF_ERROR(-1 != sem_close(wd_args.sem), "sem_close error", SEMCLOSE_FAIL);

    WDSchedulerManage(&wd_args);

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
    UNUSED(params); /*getppid not need to save pid to get it -> kill*/

    printf("..\n");
    return 0;
}


int WDSchedulerManage(wd_args_t *wd_args_)
{
    int status = 0;
    Uid_t task_uid;
    scheduler_t *wd_sched = NULL;
    struct sigaction actions[2] = {0};

    printf("\n\nWDSchedulerManage START1 Process\n\n");

    assert(wd_args_);

    /*Set sigaction */
    actions[0].sa_handler  = SIGUSR1Handler;
    actions[1].sa_handler = SIGUSR2Handler;

    /*call sigaction function to determine SIGUSR1 and SIGUSR2 handler*/
    RETURN_IF_ERROR(-1 != sigaction(SIGUSR1, &actions[0], NULL), "sigaction", 
                                                                SIGADD_FAIL);

    RETURN_IF_ERROR(-1 != sigaction(SIGUSR2, &actions[1], NULL), "sigaction", 
                                                                SIGADD_FAIL);
    /*Create scheduler*/
    wd_sched = SchedCreate();
    ExitIfError(NULL != wd_sched, "Failed to create a WatchDog Scheduler!\n", CREATE_FAIL);

    if (wd_args_->is_user_prog)
    {
        /*TASK: Resucitate if its user process*/
        task_uid = SchedAdd(wd_sched, (int (*)(void *))Resucitate, wd_args_, 0);
        RETURN_IF_ERROR(!UidIsEqual(task_uid, UID_BAD), "SchedAdd fail", 
                                                                    ADD_FAIL);
    }
    else
    {
        wd_args_->signal_pid = getppid();

        /*TASK: in case the inner watchdog app is running add a task to the scheduler 
        is the first to send a signal because the parent is definitely ready 
        to receive signals*/	
        task_uid = SchedAdd(wd_sched, (int (*)(void *))FirstSignal, wd_args_, 0);
            
        RETURN_IF_ERROR(!UidIsEqual(task_uid, UID_BAD), "SchedAdd fail", 
                                                                    ADD_FAIL);
    }


    /*TASK: check if global counter = max fail*/
    task_uid = SchedAdd(wd_sched, (int (*)(void *))SigSenderAndCountChecker, wd_args_, wd_args_->signal_intervals);
	RETURN_IF_ERROR(!UidIsEqual(task_uid, UID_BAD), "SchedAdd fail", ADD_FAIL);

    status = SchedRun(wd_sched);
	RETURN_IF_ERROR(1 == status, "SchedRun fail", ADD_FAIL);

    SchedDestroy(wd_sched);

	if (wd_args_->is_user_prog)
	{
		while( -1 == kill(wd_args_->signal_pid, SIGUSR2))
        {
            kill(wd_args_->signal_pid, SIGUSR2);
        }
	}
    
    /*	return if scheduler has successfully finished */
    return (ILRD_SUCCESS == status ? ILRD_SUCCESS : ILRD_FALSE);
}


static void SIGUSR1Handler(int sig_)
{
	assert(sig_ == SIGUSR1);

	g_fail_counter = 0;
	g_is_child_ready = 1;		
}	


static void SIGUSR2Handler(int sig_)
{
    assert(sig_ == SIGUSR2);

	UNUSED(sig_);
	
	g_is_not_resucitate = 1;
}

int BlockAllSignalsHandler(void) 
{
    sigset_t all_signals;
    
    /*Initialize the set to include all possible signals*/
    if (sigfillset(&all_signals) != 0) 
     RETURN_IF_ERROR(sigfillset(&all_signals) != 0, "sigprocmask error", ILRD_FALSE);

    /*Block all signals*/
    if (sigprocmask(SIG_BLOCK, &all_signals, NULL) != 0) 
    RETURN_IF_ERROR(sigprocmask(SIG_BLOCK, &all_signals, NULL) != 0,
                                                    "sigprocmask error", ILRD_FALSE);
    
    return 0; /*BlockAll sucsessfully*/
}

int UnBlockSignalHandler(void)
{
    sigset_t mask_set;

    /*Block all signals*/
    /*Initialize an empty signal mask_set*/
    RETURN_IF_ERROR(0 == sigemptyset(&mask_set), "sigemptyset error", ILRD_FALSE);

    /*Add SIGUSR1 and SIGUSR2 to the mask set*/
    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR1), "sigaddset error", ILRD_FALSE);

    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR2), "sigaddset error", ILRD_FALSE);
        
    /*Block mask_set (SIGUSR1, SIGUSR2)*/
    sigprocmask(SIG_UNBLOCK, &mask_set, NULL);

    return 0; /*UnBlock sucsessfully*/
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

