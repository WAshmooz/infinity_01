/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			16.08.23 
****Last update:  		16.08.23
****Version:			1
****Description:		Ping Pong 

**************************	Header & Macro	********************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct 
{
	void* hendler_fun_ptr; 
}sigaction_t;
/***************************	Prototypes	********************************/
void handle_signal(int signum);

/********************************	Main	********************************/
int g_flag = 0;

int main() 
{
	sigaction_t sigaction_struct;

	/*TODO*/
	
    sigaction(SIGUSR1, &sigaction_struct, NULL);
    sigaction(SIGUSR2, &sigaction_struct, NULL);

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
            kill(getppid(), SIGUSR2);
        }
    } 
    
    else if (child_pid > 0) 
    {
        /*Parent process*/
        printf("Parent sending SIGUSR1\n");
        
        while (1) 
        {
        	while(!g_flag)
        	{
            	 pause(); /*Parent waits for signal from child*/
            }
            
            g_flag = 0;
            kill(child_pid, SIGUSR1);
        }
    }
    } 
    
    else 
    {
        perror("Fork failed");
        return 1;
    }

    return 0;
}
/***************************	Functions	********************************/
void handle_signal(int signum) 
{
    if (getpid() == child_pid) 
    {
        if (signum == SIGUSR1) 
        {
            printf("Child received SIGUSR1\n");
            g_flag = 0;
        }
    } 
    
    else 
    {
        if (signum == SIGUSR2) 
        {
            printf("Parent received SIGUSR2\n");
            
            g_flag = 1;
        }
        }
    }
}




