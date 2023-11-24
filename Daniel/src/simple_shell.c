/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			Amit
****Creation: 			15.08.23 
****Last update:  		16.08.23
****Version:			1
****Description:		Simple Shell 

**************************	Header & Macro	********************************/
#include <assert.h>     /*assert*/
#include <stdio.h>		/*fprintf, scanf*/
#include <stdlib.h>		/*system */
#include <string.h>		/*strcmp */
#include <unistd.h>

#include <sys/types.h>  /*fork */
#include <sys/wait.h>  /*wait  */

#define HANDLE_ERROR(condition, message) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Error: %s\n", (message)); \
        } \
    } while (0)


HANDLE_ERROR(some_condition, "An error occurred");

    
enum {BUF_SIZE  = 1024, MAX_ARG_COUNT = 64};

typedef int (*fun_ptr_t) (const char *cmd); 

/***************************	Prototypes	********************************/
static int UsingFork(const char *cmd_);
static int SimpleShell(const char *syscall);

/******************************** Main ********************************/
int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        printf("USAGE %s <fork/system>\n", argv[0]);
        exit(0);
    }
    SimpleShell(argv[1]);
    return 0;
}

/*************************** Functions ********************************/
static int UsingFork(const char *cmd_) 
{
    pid_t child_pid = 0;
    char *argv[MAX_ARG_COUNT] = { NULL };
    int i = 0;
    char cmd[BUF_SIZE];
    strcpy(cmd, cmd_);

    argv[0] = strtok(cmd, " ");

    for (i = 1; i < MAX_ARG_COUNT - 1; ++i) 
    {
        argv[i] = strtok(NULL, " ");
        if (!argv[i]) 
        {
            break;
        }
    }

    /* Duplicating the process */
    child_pid = fork();
    IF_BAD(child_pid >= 0, "The fork system call failed", __LINE__);

    /* Wait and clean up for the parent process */
    if (child_pid) 
    {
        IF_BAD(waitpid(child_pid, NULL, 0) > 0 ,"Waiting and cleaning up the child process failed", __LINE__);
    }
    /* The child process */
    else 
    {
        execvp(argv[0], argv);
        perror("Child process creation failed");
        exit(EXIT_FAILURE);
    }

    return child_pid;
}

static int SimpleShell(const char *syscall) 
{
    char buffer[BUF_SIZE];
    fun_ptr_t chosen_fun = NULL;

    assert(!strcmp(syscall, "fork") || !strcmp(syscall, "system"));

    if (!strcmp(syscall, "fork")) 
    {
        chosen_fun = UsingFork;
    } 
    
    else 
    {
        chosen_fun = system;
    }

    while (1) 
    {
        printf("Shell> ");
        if (!fgets(buffer, BUF_SIZE, stdin)) 
        {
            break; /* Exit fgets failed */
        }
        
        buffer[strlen(buffer) - 1] = '\0'; /* Remove newline character */
        if (!strcmp(buffer, "exit")) 
        {
            break; /* Handle exit command */
        }

        /* Calling the chosen function */
        chosen_fun(buffer);
    }

    return 0;
}



