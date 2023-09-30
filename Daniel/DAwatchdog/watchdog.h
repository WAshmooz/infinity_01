/*******************************************************************************
* Author: Daniel Shabso
* Date: 11/09/2023
* Version: 1
* Description: Watchdog
*******************************************************************************/
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <stddef.h>    /* size_t */

/******************************************************************************
Description: 	Starts a process that checks if the calling process is running 
			 	normally; if the caller process stops running, restart it till 
			 	DoNotResuscitate called
Return value:   0 in case of successful creation of watchdog otherwise another 
				number according to happend error 	
IMPORTANT:		Uses SIGUSR1/2 signals.
				Blocks SIGUSR1/2, don't unblock them on your threads.
				call this function from the main thread.
				call DNR() before exiting.      	   						
******************************************************************************/
int MakeMeImmortal(int argc_, const char *const *argv_, size_t frequency_, 
															int fail_counter_);

/******************************************************************************
Description:	Stops control over process restart
Return value:   non-zero if fails. 	
Notes:           	
*******************************************************************************/
int DoNotResuscitate(void);


#endif    /*__WATCHDOG_H__*/