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
#include "common.h"	/* WDHandler, assert, wd_args_t, pid_t, assert, 
						exit, atoi, puts									*/
#define SEM_NAME "/wd_semaphore"



/****************************Function prototypes*******************************/
int *WatchdogThread(wd_args_t *wd_args_);
int BlockAllSignalsHandler(void); /*Block all signals*/
int BlockSignalHandler(void);   /*Block SIGUSR1, SIGUSR2*/
int UnBlockSignalHandler(void); /*UnBlock SIGUSR1, SIGUSR2*/
static void SignalCountHandle(int signum_);
int WDSchedulerManage(wd_args_t *wd_args_);
static int StamScheduler(void *params);
void printWdArgs(const wd_args_t *args);
int DoNotResuscitate(void);
/******************************Global variables********************************/
volatile int g_fail_counter;
volatile int g_is_not_resucitate;
volatile int g_is_child_ready;

extern pthread_t tid; 
pid_t wd_process_id;
char buf_max_fails[10]; 
char buf_signal_intervals[10];

/**********************************Functions***********************************/

/*User function to initialize the watchdog*/
int MakeMeImurtal(int argc_, char *argv_[], size_t signal_intervals,
                                                             size_t max_fails) 
{
    int status = 0;
    wd_args_t *wd_args = NULL;

    status = BlockSignalHandler();
    RETURN_IF_ERROR(0 == status, "BlockSignalHandler error", status);

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

    /*Set wd_args*/
    wd_args->argv_list[0] = "./watchdog.out";
    wd_args->argv_list[1] = buf_max_fails;
    wd_args->argv_list[2] = buf_signal_intervals;

    memcpy(wd_args->argv_list + 3, argv_, argc_ * sizeof(char *));

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

    printf("MakeMeImurtal after wait\n");


    return 0;
}

int DNR(void)
{
    printf("\n  DNR  \n");
    return (0);
}

int *WatchdogThread(wd_args_t *wd_args_)
{	
    pid_t pid;
    int i = 0;

    sigset_t signal_set = {0};
    char *args[] = {"./watchdog.out", NULL};
    int ststus_sigaction = 0; /* SUCCESS */
    struct sigaction handler = {0};	

    /*Block all signals and Unblock SIGUSR1 and SIGUSR2 */
    ExitIfError(0 == BlockAllSignalsHandler(),
                        "Error: BlockAllSignalsHandler failed", ILRD_FALSE);
    ExitIfError(0 == UnBlockSignalHandler(),
                        "Error: UnBlockSignalHandler failed", ILRD_FALSE);

    /* Create signal handler for SIGUSR1 */
    handler.sa_handler = &SignalCountHandle;
    ststus_sigaction = sigaction(SIGUSR1, &handler, NULL);
    ExitIfError(ILRD_SUCCESS == ststus_sigaction,
                                     "Error: sigaction1 failed", ILRD_FALSE); 

    printf("Fork START\n");
    printWdArgs(wd_args_);

    pid = fork(); 
    if (0 == pid)
    {
        execvp(wd_args_->argv_list[0], wd_args_->argv_list);
        printf("WE ARE NOT SUPPOSED TO GET HERE\n");
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


int WDSchedulerManage(wd_args_t *wd_args_)
{
    int ret_status = 0;
    scheduler_t *wd_scheduler = NULL;

    printf("\n\nWDSchedulerManage START Thread\n\n");

    /*Create scheduler*/
    wd_scheduler = SchedCreate();
    ExitIfError(NULL != wd_scheduler,
                             "Failed to create a WatchDog Scheduler!\n", -1);

    /*SchedAdd(wd_scheduler, SendSignal, NULL, 1);*/
    /*ExitIfError(UID_BAD == SchedAdd(wd_scheduler, SendSignal, NULL, 1),
                                             "Failed to SchedAdd!\n", -1);*/
    SchedAdd(wd_scheduler, StamScheduler, wd_args_, wd_args_->signal_intervals);

    ret_status = SchedRun(wd_scheduler);

    SchedDestroy(wd_scheduler);

    /*	return if scheduler has successfully finished */
    return (ILRD_SUCCESS == ret_status ? ILRD_SUCCESS : ILRD_FALSE);
}



static int StamScheduler(void *params_)
{
    wd_args_t *wd_args = (wd_args_t *)params_;
    if (wd_args->is_user_prog == IS_USER_PROG)
    {
        printf("USER APP SENDING SIGNAL (StamScheduler)\n");
    }
    else{
        printf("WD PROCESS SENDING SIGNAL (StamScheduler)\n");
    }
    kill(wd_args->signal_pid, SIGUSR1);

    return 0;
}

int BlockAllSignalsHandler(void) 
{
    sigset_t all_signals;
    
    /*Initialize the set to include all possible signals*/
     RETURN_IF_ERROR(sigfillset(&all_signals) == 0,
                                             "sigprocmask error", ILRD_FALSE);

    /*Block all signals*/
    RETURN_IF_ERROR(sigprocmask(SIG_BLOCK, &all_signals, NULL) == 0,
                                            "sigprocmask error", ILRD_FALSE);
    
    return 0; /*BlockAll sucsessfully*/
}

int BlockSignalHandler(void)
{
    sigset_t mask_set;
    
    /*Initialize an empty signal mask_set*/
    RETURN_IF_ERROR(0 == sigemptyset(&mask_set), "sigemptyset error", ILRD_FALSE);

    /*Add SIGUSR1 and SIGUSR2 to the mask set*/
    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR1), "sigaddset error", ILRD_FALSE);

    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR2), "sigaddset error", ILRD_FALSE);
        
    /*Block mask_set (SIGUSR1, SIGUSR2)*/
    sigprocmask(SIG_BLOCK, &mask_set, NULL);

    return 0; /*Block sucsessfully*/

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

/* Helper function for PRINT ONLY */
void printWdArgs(const wd_args_t *args) 
{
    int i = 0;
    printf("wd_args:\n");
    printf("  argv_list:\n");

    while (NULL != args->argv_list[i]) 
    {
        printf("    argv_list[%d]: %s\n", i, args->argv_list[i]);
        ++i;
    }
}

static void SignalCountHandle(int signum) 
{
    printf("                [%zu]handlerFun_of_process\n", (size_t)pthread_self());

    if (signum == SIGUSR1) 
    {
        g_fail_counter = 0;
    }
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











