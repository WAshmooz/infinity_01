/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			16.08.23 
****Last update:  		16.08.23
****Version:			1
****Description:		Ping Pong 

**************************	Header & Macro	********************************/
#define _POSIX_SOURCE /*struct sigation*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


/***************************	Prototypes	********************************/
void HandleSignal(int signum);

/********************************	Main	********************************/
int g_flag = 0;

int main() 
{
	int ststus_sigaction = 0;
	struct sigaction handler = {0};
	pid_t child_pid = 0;
	
	handler.sa_handler = &HandleSignal;
	
    ststus_sigaction = sigaction(SIGUSR1, &handler, NULL);
    ststus_sigaction = sigaction(SIGUSR2, &handler, NULL);

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
            printf("Child sending SIGUSR2\n");
            kill(getppid(), SIGUSR2);
        }
    } 
    
    else if (child_pid > 0) 
    {
    	int count = 5;
        /*Parent process*/
        
        while (count) 
        {
        	while(g_flag )
        	{
        		--count;
            	 pause(); /*Parent waits for signal from child*/
            }
            
            g_flag = 1;
            printf("Parent sending SIGUSR1\n");
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
        printf("Child received SIGUSR1\n");
        g_flag = 1;
    }
	/*Parent process*/   
    else /*signum == SIGUSR2*/ 
    {
            printf("Parent received SIGUSR2\n");
            
            g_flag = 0;
    }
}




