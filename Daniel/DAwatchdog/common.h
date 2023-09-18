/*******************************************************************************
* Author: Daniel Shabso
* Date: 11/09/2023
* Version: 1
* Description: Watchdog common functions
*******************************************************************************/
#ifndef __WD_COMMON_H__
#define __WD_COMMON_H__

#include <assert.h>		/* assert*/
#include <stddef.h>		/* size_t*/
#include <stdio.h> 		/* printf, perror*/
#include <stdlib.h>		/* exit, atoi*/
#include <semaphore.h>	/*	sem_op	*/

#include <sys/types.h> 	/* pid_t*/

#include "aux_funcs.h"	/* RETURN_IF_ERROR, ExitIfError	*/

#define SEM_NAME "/wd_semaphore"

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
    int argc;
	size_t signal_intervals;
	size_t max_fails;
	int is_user_prog;
	pid_t signal_pid;
	sem_t *sem; 

} wd_args_t;


#ifdef _DEBUG
	#define DEBUG_ONLY(expression) expression
#else
	#define DEBUG_ONLY(expression)
#endif



/******************************************************************************/

int WDHandler(wd_args_t *args_);

int SignalMaskHandler(void);


#endif   /*__WD_COMMON_H__*/
