/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog (simple pase)

**************************	Header & Macro	********************************/

#include <assert.h>
#include <string.h>/*strcat, strlen*/
#include <stdio.h> /*fprintf, scanf*/
#include <unistd.h> /*pid_t, fork, execvp, getcwd*/
#include <errno.h>

#define _POSIX_SOURCE /*include expand library of signal.h (sigation)*/

#include <signal.h> /*sigaction*/
#include <sys/types.h>
#include <sys/wait.h>/* waipid*/

#include "aux_funcs.h"
#include "scheduler.h"/* schedular_t, SchedCreate, SchedAdd, SchedRun*/
#include "uid.h"/* Uid_t */


int WDHandler(wd_args_t *args_)
{
	Uid_t task_uid;
	scheduler_t *sched = NULL;
	struct sigaction actions[2] = {0};
	
	assert(args_);
	
	/*change mode from handler to action*/
	actions[0].sa_flags = SA_SIGINFO;
	actions[0].sa_sigaction = SIGUSR1Handler;
	
	actions[1].sa_handler = SIGUSR2Handler;
	
	/*call sigaction function to determine SIGUSR1 and SIGUSR2 handler*/
	RETURN_IF_ERROR(-1 != sigaction(SIGUSR1, &actions[0], NULL), "sigaction", 
																SIGADD_FAIL);
	
	RETURN_IF_ERROR(-1 != sigaction(SIGUSR2, &actions[1], NULL), "sigaction", 
																SIGADD_FAIL);
	/*create Scheduler*/
	sched = SchedCreate();
	
	/*check if Scheduler creation was successful */
	RETURN_IF_ERROR(NULL != sched, "SchedCreate fail", CREATE_FAIL);
	
	if (args_->is_user_prog)
	{
		task_uid = SchedAdd(sched, (int (*)(void *))Resucitate, args_, 0);
			
		RETURN_IF_ERROR(!UidIsEqual(task_uid, UID_BAD), "SchedAdd fail", 
																	ADD_FAIL);
	}
	else
	{
		args_->signal_pid = getppid();
	
		/*in case the inner watchdog app is running add a task to the scheduler 
		is the first to send a signal because the parent is definitely ready 
		to receive signals*/	
		task_uid = SchedAdd(sched, (int (*)(void *))FirstSignal, args_, 0);
			
		RETURN_IF_ERROR(!UidIsEqual(task_uid, UID_BAD), "SchedAdd fail", 
																	ADD_FAIL);
	}
	
	/*add task that check if global counter >= fail_counter*/
	task_uid = SchedAdd(sched, (int (*)(void *))SigSenderAndCountChecker, args_, 
															args_->frequency);
		
	RETURN_IF_ERROR(!UidIsEqual(task_uid, UID_BAD), "SchedAdd fail", ADD_FAIL);
	
	SchedRun(sched);
	
	if (args_->is_user_prog)
	{
		ExitIfError(-1 != kill(args_->signal_pid, SIGUSR2), "kill error", 
																	KILL_FAIL);
	}
	
	/*destroy Scheduler*/
	SchedDestroy(sched);
	
	return NORMAL_FLOW;
}