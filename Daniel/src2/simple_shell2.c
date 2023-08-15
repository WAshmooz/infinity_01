#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARG_COUNT 64

int main() 
{
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_ARG_COUNT];
    int use_fork = 1; /*Default to using fork*/

    while (1) 
    {
    	char *token = NULL;
        int arg_count = 0;
        printf("Shell> ");

        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) 
        {
            break; /*Exit on Ctrl+D or error*/
        }

        input[strlen(input) - 1] = '\0'; /*Remove newline character*/

        if (strcmp(input, "exit") == 0) 
        {
            break; /*Handle exit command*/
        }

        /*Ask the user whether to use fork or system*/
        printf("Use fork (1) or system (0)? ");
        scanf("%d", &use_fork);
        getchar(); /*Consume the newline character*/

        /*Tokenize the input*/
        token = strtok(input, " ");
        arg_count = 0;

        while (token != NULL) 
        {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }

        args[arg_count] = NULL; /*NULL-terminate the argument list*/

        if (use_fork) 
        {
            /*Using fork*/
            pid_t pid = fork();

            if (pid == 0) 
            {
                /*Child process*/
                if (execvp(args[0], args) == -1) 
                {
                    perror("Error executing command");
                }
                exit(EXIT_FAILURE);
            } 
            
            else if (pid < 0) 
            {
                perror("Error forking");
            } 
            
            else 
            {
                /*Parent process*/
                wait(NULL);
            }
        } 
        
        else 
        {
            /*Using system*/
            int status = system(input);
            if (status == -1) 
            {
                perror("Error executing command");
            }
        }
    }

    return 0;
}
