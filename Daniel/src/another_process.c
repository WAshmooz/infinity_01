/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			16.08.23 
****Last update:  		16.08.23
****Version:			1
****Description:		Ping Pong 

**************************	Header & Macro	********************************/
#define _POSIX_SOURCE /*include expand library of signal.h (sigation)*/
#include <stdio.h>		/*fprintf, scanf*/
#include <stdlib.h>		/*system */
#include <unistd.h>		/*pid_t*/

#include <sys/types.h>  /*fork*/
#include <sys/wait.h>  /*pause*/

#include <signal.h>		/*sigaction*/

#define RETURN_IF_BAD(condition, error_message, return_val) \
do { \
    if (!(condition)) { \
        fprintf(stderr, "Error: %s (Line %d)\n", error_message, __LINE__); \
        return return_val; \
    } \
} while (0)

enum {PROCESS_FAIL = -1 ,PROCESS_SUCCESS = 0} ;
static volatile int g_flag = 0;
/***************************	Prototypes	********************************/
void HandleSignal(int signum);

/********************************	Main	********************************/


int main() 
{
	int ststus_sigaction = 0; /*SUCCESS*/
	struct sigaction handler = {0};
	pid_t child_pid = 0;
	
	handler.sa_handler = &HandleSignal;
	
    ststus_sigaction = sigaction(SIGUSR1, &handler, NULL);
    RETURN_IF_BAD(PROCESS_SUCCESS == ststus_sigaction, "Eror: sigaction1 failed", PROCESS_FAIL);
    
    ststus_sigaction = sigaction(SIGUSR2, &handler, NULL);
    RETURN_IF_BAD(PROCESS_SUCCESS == ststus_sigaction, "Eror: sigaction1 failed", PROCESS_FAIL);

    child_pid = fork();
	
    if (child_pid == 0) 
    {
        /*Child process*/
        g_flag = 1;
        
        while (1) 
        {
        	while(!g_flag)
        	{
            	 pause(); /*Child waits for signal from parent*/
            }
            
            g_flag = 0;
            printf("Pong\n"); /*Child sending SIGUSR2*/
            kill(getppid(), SIGUSR2); /*Child term*/
        }
    } 
    
    else if (child_pid > 0) 
    {
    	int count = 20;
        /*Parent process*/
        
        while (count) 
        {
        	while(g_flag )
        	{
        		--count;
            	 pause(); /*Parent waits for signal from child*/
            }
            
            g_flag = 1;
            printf("Ping\n"); /*arent sending SIGUSR1*/
            kill(child_pid, SIGUSR1);
        }
    } 
    
    else 
    {
        perror("Fork failed");
        return 1;
    }
	
	kill(child_pid, SIGTERM);
	
    return 0;
}
/***************************	Functions	********************************/
void HandleSignal(int signum) 
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
