/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		03.09.23
****Version:			1
****Description:		Watchdog 

**************************	Header & Macro	********************************/
#define _POSIX_SOURCE /*include expand library of signal.h (sigation)*/
#include <stdio.h> /*fprintf, scanf*/
#include <stdlib.h> /*system */
#include <unistd.h> /*pid_t*/
#include <pthread.h>
#include <sys/types.h>
#include <signal.h> /*sigaction*/

typedef struct 
{
	char **arg_list;
	volatile size_t sec_bet_signals;
	volatile size_t f_count;
} my_args_t;
/***********************	Function Declerations	**************************/
int MMakeMeImurtalMI(int argc, char *argv[], size_t sec_bet_signals, size_t fail_count);
int DNR(void);
static void HandleSignal1(int signum);
static void HandleSignal2(int signum);
void* watchdog_function(void* arg) 

/***************************    Main    **************************************/
static volatile int g_count = fail_count;


int MakeMeImurtal(int argc, char *const*argv[], size_t sec_bet_signals, size_t fail_count);
{    
    /*SIGMASK --> BLOCK ALL*/

    /*char *arg_list -> add argv[] + freq + f_cound*/

    /*crate arg struct contains the params we need for exec*/

    /*CREATE THREAD*/

    /*WAIT(we can use sleep as a start)*/
}    



static void *WatchdogThread(my_args_t *my_args)
{
    /*SIGMASK -> UNBLOCK SIGUSR1 / SIGUSER2*/
    
    /*create SIGACTION1 --> reset count*/

    /*create SIGACTION2 --> send signal when watchdog is ready*/

	/*create scheduler*/

	/*ADD TaskSendAndCheck():
	{
		send signal1
		if signal fail cuse process fall (check by errno == ESRCH)
		{
            TODO (maybe need to fork and exec)
		}

		if (g_count == fail_count)
		{
			if (is_user_thread)
            {
                fork();
            } 
            exec(executble from params);
		}

        ++g_count;
	}
	*/

    /*run scheduler*/

    /*pid_child = fork()*/

    if (is_user_thread)  /*CHILD_PROCESS*/
    {
         execv(WatchdogProcess, my_args->arg_list)
    } 
    else    /*PARENT_PROCESS*/
    {
        /**/
    }
}

int main() /*WatchdogProcess*/
{	
    char* client_program = argv[1];
	int frequency = atoi(argv[argc-2]);
	int g_fail_count = atoi(argv[argc-1]);

    while(env)
    {the
        '_-[]0
        'i;5'
    }

}



/*User function to stop the watchdog*/
int DNR(void) 
{
    while(1)
    {
        /*Terminate the watchdog thread*/
        if (env)
        {
            /*kill process + thread*/
            pthread_cancel(WatchdogThread);
        }
    }

	return 0;
}

/******************************************************************************
 *                            HELPER FUNCRIONS 
*******************************************************************************/
static void HandleSignal(int signum) 
{
    if (signum == SIGUSR) 
    {
        g_count = 0;
    }
}

static void HandleSignal(int signum) 
{
	/*Child process*/
    if (signum == SIGUSR1) 
    {
        g_flag = 1;
    }
	/*Parent process*/   
    else /*signum == SIGUSR2*/ 
    {
            g_flag = 0;
    }
}











