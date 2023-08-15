#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 1024

int ForkNexec(const char* cmd);

int main(int argc, char *argv[]) 
{
    int (*exe_func)(const char*) = NULL;
    
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <system|fork>\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "system") == 0) 
    {
        exe_func = system;
    } 
    
    else if (strcmp(argv[1], "fork") == 0) 
    {
        exe_func = ForkNexec;
    } 
    
    else 
    {
        fprintf(stderr, "Invalid argument. Use 'system' or 'fork'.\n");
        exit(1);
    }

    while (1) 
    {
        char cmd[MAX_INPUT_LENGTH];
        printf("Shell> ");
        
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) 
        {
            perror("Error reading input");
            exit(1);
        }
        
        /*Remove the newline character from the command*/
        cmd[strcspn(cmd, "\n")] = '\0';
        
        if (strcmp(cmd, "exit") == 0) 
        {
            printf("Exiting the shell.\n");
            exit(0);
        } 
        
        else 
        {
            int status = exe_func(cmd);
            
            if (status != 0) 
            {
                fprintf(stderr, "Command execution failed with status %d.\n", status);
            }
        }
    }

    return 0;
}

int ForkNexec(const char* cmd) 
{
    pid_t pid = fork();
    
    if (pid < 0) 
    {
        perror("Fork failed");
        return 1;
    } 
    
    else if (pid == 0) 
    {   
    	/*Child process*/
        char* args[] = {"/bin/sh", "-c", (char*)cmd, NULL};
        execv("/bin/sh", args);
        
        perror("Exec failed");
        exit(1);
    }
    
     else 
     { 
     	/*Parent process*/
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
}


















