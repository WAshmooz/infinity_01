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
static void InitArgs(wd_args_t *args_, char **arr_args_, char *arr_interval_, 
														char *arr_max_fail_);


static int SigSenderAndCountChecker(wd_args_t *args_);
static int FirstSignal(wd_args_t *args_);
static void SIGUSR1Handler(int sig_);
static int Resucitate(wd_args_t *args_);
static void SIGUSR2Handler(int signo_);
/******************************Global variables********************************/
volatile int g_fail_counter;
volatile int g_is_not_resucitate;
volatile int g_is_child_ready;

extern pthread_t tid; 
pid_t wd_process_id;
char buf_max_fails[10]; 
char buf_signal_intervals[10];

enum 
{
	ARR_ARGV_SIZE = 20,
	ARR_FNAME_SIZE = 100,
	ARR_NUM_SIZE = 32
};
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
    ExitIfError(NULL != wd_sched,
                     "Failed to create a WatchDog Scheduler!\n", CREATE_FAIL);

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

        /*TASK: in case the inner watchdog app is running add a task to the
        scheduler is the first to send a signal because the parent is definitely 
        ready to receive signals*/	
        task_uid = SchedAdd(wd_sched, (int (*)(void *))FirstSignal, wd_args_, 0);
            
        RETURN_IF_ERROR(!UidIsEqual(task_uid, UID_BAD), "SchedAdd fail", 
                                                                    ADD_FAIL);
    }


    /*TASK: check if global counter = max fail*/
    task_uid = SchedAdd(wd_sched, (int (*)(void *))SigSenderAndCountChecker,
                                         wd_args_, wd_args_->signal_intervals);
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


static int Resucitate(wd_args_t *args_)
{
	pid_t child_pid = 0;
	
	if (args_->is_user_prog)
	{
		int status = 0;

		char *arr_args[ARR_ARGV_SIZE] = {NULL};
		char arr_freq[ARR_NUM_SIZE] = {0};
		char arr_f_c[ARR_NUM_SIZE] = {0};

		InitArgs(args_, arr_args, arr_freq, arr_f_c);
			
		if (kill(args_->signal_pid, 0) == 0) 
		{
			kill(args_->signal_pid, SIGKILL);
			
			/*waiting for the process to clean up*/
			waitpid(args_->signal_pid, &status, 1);
        }
        
		child_pid = fork();
		
		ExitIfError(-1 != child_pid, "fork error", FORK_FAIL);
	
		if (!child_pid) 
		{
			execvp(arr_args[0], arr_args);

			perror("execvp error");  
			exit(EXECUTION_FAIL);
		}
		
		else
		{
			/*DEBUG_ONLY(LogI("fork pid %d", child_pid));*/
			
			g_fail_counter = 0;
			g_is_child_ready = 0;
			
			args_->signal_pid = child_pid;
		}
    }

	else 
	{
		kill(args_->signal_pid, SIGKILL);

		execvp(args_->argv_list[0], (char **)args_->argv_list);

		/*LogE("execvp error");  */
		exit(EXECUTION_FAIL);
	}
	
	/*return 1 to finish the task and not reschedule it*/
	return 1;
}

static int SigSenderAndCountChecker(wd_args_t *args_)
{
	if (g_is_not_resucitate)
	{
		return 1;
	}

	/*check if given counter >= fail_counter*/
	if (g_fail_counter >= args_->max_fails)
	{
		/*call exec or fork/exec function*/
		Resucitate(args_);
	}
	
	else
	{
		if (args_->is_user_prog) 
		{
			/*flag wait for first signal from the main*/
			if (g_is_child_ready) 
			{
				if (-1 == kill(args_->signal_pid, SIGUSR1))
				{
					if (ESRCH != errno)
					{
						perror("kill error");
						exit(KILL_FAIL);
					}
				}
			}
		} 

		else 
		{
			if (-1 == kill(args_->signal_pid, SIGUSR1))
			{
				if (ESRCH != errno)
				{
					exit(KILL_FAIL);
				}
			}
		}
		/*increase value of the global counter*/
		++g_fail_counter;
	}
	
	/*return 0 for rescheduling*/
	return 0;
}


static void SignalCountHandle(int signum) 
{
    printf("                [%zu]handlerFun_of_process\n", (size_t)pthread_self());

    if (signum == SIGUSR1) 
    {
        g_fail_counter = 0;
    }
}

static int FirstSignal(wd_args_t *args_)
{
	UNUSED(args_);
    printf("                                        FirstSignal\n");
	/*send siganl to parent id*/
	ExitIfError(-1 != kill(args_->signal_pid, SIGUSR1), "kill error", KILL_FAIL);
	
	/*return 1 to finish the task and not reschedule it*/
	return 1;
}

/*g_fail_counter = 0, g_is_child_ready = 1*/
static void SIGUSR1Handler(int sig_)
{
	assert(sig_ == SIGUSR1);
    printf("%60d signal sending SIGUSR1 from: THREAD [pid = %d]\n", getpid());
	g_fail_counter = 0; 
	g_is_child_ready = 1;		
}	

/*g_is_not_resucitate = 1*/
static void SIGUSR2Handler(int sig_)
{
    assert(sig_ == SIGUSR2);

	UNUSED(sig_);
	
	g_is_not_resucitate = 1;
}

static void InitArgs(wd_args_t *args_, char **arr_args_, char *arr_interval_, 
															char *arr_max_fail_)
{
	int i = 0;

	sprintf(arr_interval_, "%ld", args_->signal_intervals);
	sprintf(arr_max_fail_, "%ld", args_->max_fails);
	
	arr_args_[0] = "./main_wd.out";
	arr_args_[1] = arr_interval_;
	arr_args_[2] = arr_max_fail_;
	
	while (NULL != args_->argv_list[i])
	{
		arr_args_[i + 3] = (char *)args_->argv_list[i];
        ++i;
	}
}









