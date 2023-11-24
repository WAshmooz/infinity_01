/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog (common functions)


**************************	Header & Macro	***********************************/
#define _POSIX_C_SOURCE /*include expand library of signal.h (sigation)*/
#define _POSIX_SOURCE /*include expand library of signal.h (sigation)*/
#include <assert.h> /*assert*/
#include <stdio.h> /*fprintf, scanf, snprintf*/
#include <stdlib.h> /*system */
#include <unistd.h> /*pid_t*/
#include <pthread.h> 
#include <sys/types.h>
#include <signal.h> /*sigaction*/
#include <semaphore.h>	/*sem_op*/
#include <string.h> /*memcpy*/
#include <fcntl.h>/*O_CREAT for sem_op*/
#include <errno.h>

#include "scheduler.h"
#include "aux_funcs.h"
#include "common.h"	/* WDHandler, assert, wd_args_t, pid_t, assert, 
						exit, atoi, puts									*/

static volatile int g_fail_counter;
static volatile int g_is_not_resucitate;
static volatile int g_is_child_ready;

/**********************************Functions***********************************/

 int WDSchedulerManage(wd_args_t *wd_args_)
{
    int status = 0;
    Uid_t task_uid;
    scheduler_t *wd_sched = NULL;
    struct sigaction actions[2] = {0};

    DEBUG printf("\nWDSchedulerManage START1 Process\n");

    assert(wd_args_);

    /*Set 2 sigactions */
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
        task_uid = SchedAdd(wd_sched,(int (*)(void *))FirstSignal, wd_args_, 0);
            
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
        DEBUG printf("USER APP killed by \n");
		while( -1 == kill(wd_args_->signal_pid, SIGUSR2))
        {
            
            kill(wd_args_->signal_pid, SIGUSR2);
        }
        
	}
    
    /*	return if scheduler has successfully finished */
    return (ILRD_SUCCESS == status ? ILRD_SUCCESS : ILRD_FALSE);
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

 int BlockSIGUSR12Handler(void)
{
    sigset_t mask_set;
    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR1),
                                                 "sigaddset error", ILRD_FALSE);

    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR2),
                                                 "sigaddset error", ILRD_FALSE);
        
    /*Block mask_set (SIGUSR1, SIGUSR2)*/
    sigprocmask(SIG_BLOCK, &mask_set, NULL);

    return 0; /*Block sucsessfully*/

}

 int UnBlockSIGUSR12Handler(void)
{
    sigset_t mask_set;

    /*Block all signals*/
    /*Initialize an empty signal mask_set*/
    RETURN_IF_ERROR(0 == sigemptyset(&mask_set),
                                             "sigemptyset error", ILRD_FALSE);

    /*Add SIGUSR1 and SIGUSR2 to the mask set*/
    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR1),
                                                "sigaddset error", ILRD_FALSE);

    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR2),
                                                 "sigaddset error", ILRD_FALSE);
        
    /*Block mask_set (SIGUSR1, SIGUSR2)*/
    sigprocmask(SIG_UNBLOCK, &mask_set, NULL);

    return 0; /*UnBlock sucsessfully*/
}

 int Resucitate(wd_args_t *args_)
{
	pid_t child_pid = 0;
	
    DEBUG printf("CALL TO Resucitate [is_user = %d]]\n", args_->is_user_prog);

	if (args_->is_user_prog)
	{
		int status = 0;
			
		if (kill(args_->signal_pid, 0) == 0) 
		{
			kill(args_->signal_pid, SIGKILL);
			
			/*waiting for the process to clean up*/
			waitpid(args_->signal_pid, &status, 1);
        }
        
		child_pid = fork();
		
		ExitIfError(-1 != child_pid, "fork error", FORK_FAIL);

		/*if its the parent it suold do exec() also*/
		if (!child_pid) 
		{
			execvp(args_->argv_list[0], args_->argv_list);

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

        DEBUG printWdArgs(args_);
		args_->argv_list[0] = args_->argv_list[3];
		args_->argv_list[1] = NULL;
		execvp(args_->argv_list[0], args_->argv_list);
        DEBUG printf("Eror Resucitate (kill & exec) \n");

		/*LogE("execvp error");  */
		exit(EXECUTION_FAIL);
	}
	
	return 1;
}

 int SigSenderAndCountChecker(wd_args_t *args_)
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

 void SignalCountHandle(int signum) 
{
    DEBUG printf("[%zu]handlerFun_of_process\n", (size_t)pthread_self());

    if (signum == SIGUSR1) 
    {
        g_fail_counter = 0;
    }
}

 int FirstSignal(wd_args_t *args_)
{
	UNUSED(args_);
    DEBUG printf("FirstSignal\n");
	/*send siganl to parent id*/
	ExitIfError(-1 != kill(args_->signal_pid, SIGUSR1),"kill error", KILL_FAIL);
	
	/*return 1 to finish the task and not reschedule it*/
	return 1;
}

/*g_fail_counter = 0, g_is_child_ready = 1*/
 void SIGUSR1Handler(int sig_)
{
	assert(sig_ == SIGUSR1);
    DEBUG printf("%40d sending SIGUSR1\n", getpid());
	g_fail_counter = 0; 
	g_is_child_ready = 1;		
}	

/*g_is_not_resucitate = 1*/
 void SIGUSR2Handler(int sig_)
{
    assert(sig_ == SIGUSR2);

	UNUSED(sig_);
	
	g_is_not_resucitate = 1;
}

/* Helper function for PRINT ONLY */
 void printWdArgs(const wd_args_t *args) 
{
    int i = 0;
    DEBUG printf("wd_args:\n");
    DEBUG printf("  argv_list:\n");

    while (NULL != args->argv_list[i]) 
    {
        DEBUG printf("    argv_list[%d]: %s\n", i, args->argv_list[i]);
        ++i;
    }
}









