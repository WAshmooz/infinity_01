/*******************************************************************************
* Author: Daniel Shabso
* Date: 11/09/2023
* Version: 1
* Description: Watchdog common functions
*******************************************************************************/
#ifndef __WD_COMMON_H__
#define __WD_COMMON_H__

/*Define _DEBUG to enable debugging code*/
#define _DEBUG
#define SEM_NAME "/wd_semaphore"

/******************************Global variables********************************/

enum 
{
	NORMAL_FLOW = 0,
	ARGS_FAIL, 
	ADD_FAIL,
	SIGADD_FAIL,
	SEMOPEN_FAIL,
	SEMPOST_FAIL,
	SEMCLOSE_FAIL,
	SEMUNLINK_FAIL,
	KILL_FAIL,
	FORK_FAIL,
	CREATE_FAIL, 
	SIGEMPTY_FAIL,
	SIGMASK_FAIL,
	SIGDEL_FAIL,
	SIGFILL_FAIL,
	EXECUTION_FAIL,
	JOIN_FAIL
};


enum 
{
	IS_WD_PROG = 0,
	IS_USER_PROG = 1,
	NUM_ARGS = 3
};

typedef struct wd_args
{
	char **argv_list;
	size_t signal_intervals;
	size_t max_fails;
	int is_user_prog; 
	pid_t signal_pid;
	sem_t *sem; 

} wd_args_t;


/*Define DEBUG_ONLY macro*/
#ifdef _DEBUG
#define DEBUG_ONLY(expression) expression
#else
#define DEBUG_ONLY(expression)
#endif	/*in the code: DEBUG_ONLY(printf("BLABLA");)*/


/******************************************************************************/
 int BlockAllSignalsHandler(void); /*Block all signals*/
 int BlockSIGUSR12Handler(void);   /*Block SIGUSR1, SIGUSR2*/
 int UnBlockSIGUSR12Handler(void); /*UnBlock SIGUSR1, SIGUSR2*/
 void SignalCountHandle(int signum_);
 int WDSchedulerManage(wd_args_t *wd_args_);
 int StamScheduler(void *params);
 void printWdArgs(const wd_args_t *args);

 int SigSenderAndCountChecker(wd_args_t *args_);
 int FirstSignal(wd_args_t *args_);
 void SIGUSR1Handler(int sig_);
 int Resucitate(wd_args_t *args_);
 void SIGUSR2Handler(int signo_);

#endif   /*__WD_COMMON_H__*/
