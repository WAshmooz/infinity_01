/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			30.08.23 
****Last update:  		30.08.23
****Version:			1
****Description:		Semaphore 

**************************	Header & Macro	********************************/
#include <stdio.h> /* printf */
#include <string.h> /*strcmp, strtok*/
#include <assert.h>  /*assert*/
#include <stdlib.h>

#include <sys/types.h> /* sem system V */
#include <sys/ipc.h>   /* sem system V */
#include <sys/sem.h>   /* sem system V */

#include "../../utils/include/utils.h" /* ExitIfError, RETURN_IF_ERROR, UNUSED */

enum
{
    BUF_SIZE = 256,
    PROJ_ID = 1234,
    ACTION_SIZE = 22
};


union semun {
    int val;
    struct semid_ds *buf;   /* IPC_STAT, IPC_SET */
    unsigned short *array;  /* GETALL, SETALL */
    struct seminfo *__buf;
} g_arg;

/*******************	Functions Declarations      *************************/
static void InputReadAndExecute(int semid);
static void DoCommand(int semid, char *cmd);
static void Tokenize(char *result_arr_[], char *str_);
static void DecreaseSemaphore(int semid, const char *const *args);
static void IncreaseSemaphore(int semid, const char *const *args);
static void ErrorFunc(); 
static void PrintValSemaphore(int semid, const char *const *args);
static void ExitSemaphore();
static int GetValSemaphore(int semid);

/***************************	Ex1     ************************************/

/*Define an Global Array of function pointers for actions*/
typedef void (*action_func_t)(int, const char *const *);

action_func_t action[ACTION_SIZE] = {
    /*C*/ DecreaseSemaphore,
    /*D*/ DecreaseSemaphore,
    /*E*/ ErrorFunc,
    /*F*/ ErrorFunc,
    /*G*/ ErrorFunc,
    /*H*/ ErrorFunc,
    /*I*/ IncreaseSemaphore,
    /*J*/ ErrorFunc,
    /*K*/ ErrorFunc,
    /*L*/ ErrorFunc,
    /*M*/ ErrorFunc,
    /*N*/ ErrorFunc,
    /*O*/ ErrorFunc,
    /*P*/ PrintValSemaphore,
    /*Q*/ ErrorFunc,
    /*R*/ ErrorFunc,
    /*S*/ ErrorFunc,
    /*T*/ ErrorFunc,
    /*U*/ ErrorFunc,
    /*V*/ ErrorFunc,
    /*W*/ ErrorFunc,
    /*X*/ ExitSemaphore};

int main(int argc, char* argv[])
{
    int semid = 0;
    key_t sem_key = 0;

    if (argc < 2) /*TODO return if bad*/ 
    {
        fprintf(stderr, "Usage: %s /semaphore_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Create key for semid */
    sem_key = ftok(argv[1], PROJ_ID); 
    if (sem_key == -1) /*TODO return if bad*/
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }   

    /* Create semid */    
    semid = semget(sem_key, 1, 0666 | IPC_CREAT);
    if (semid == -1) /*TODO return if bad*/
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    InputReadAndExecute(semid);

    return 0;
}


static void InputReadAndExecute(int semid)
{
    char cmd[BUF_SIZE] = {0};
    while (1)
    {
        printf("Waiting for input [cmd, num, undo]:\n");
        /* read input */
        fgets(cmd, BUF_SIZE, stdin);   /*TODO return if bad*/

        /* execute command */
        DoCommand(semid, cmd);
    }
}

static void DoCommand(int semid, char *cmd)
{
    char *args_tok[4] = {NULL}; /* possible format: [cmd] [number] [undo] */

    Tokenize(args_tok, cmd);

    /*Chek if input correct*/
    if ((args_tok[0][0] <= 'C') || (args_tok[0][0] >= 'X'))
    {
      ErrorFunc(); 
    }

    /*Example DecreaseSemaphore: action[0](semid, DecreaseSemaphore)*/
    action[(int)args_tok[0][0] - 'C'](semid, (const char *const *)args_tok);
}

/* converts string "str str1 str2" to an array {"str", "str1", "str2"} */
static void Tokenize(char *result_arr_[], char *str_)
{
    size_t i = 0;
    char *one_word_str = NULL;
    str_[strlen(str_)-1] = '\0';
    one_word_str = strtok(str_, " ");

    while (NULL != one_word_str)
    {
        *(result_arr_ + i) = one_word_str;
        one_word_str = strtok(NULL, " ");
        ++i;
    }
}

static void DecreaseSemaphore(int semid, const char *const *args)
{
    struct sembuf sem_args;

    /*Set new val (current + cmd)*/
    g_arg.val = GetValSemaphore(semid) - atoi(args[1]); 
    
    /*In case of getting input with undo*/
    if (args[2] && !strncmp(args[2], "undo", 4))
    {
        sem_args.sem_flg = SEM_UNDO;    /*Set undo flag*/
        sem_args.sem_op = (-1)*(atoi(args[1])); /*Decrease by val input*/

        if ( 0== sem_args.sem_op)
        {
            printf("sem_args.sem_op = 0, flag set to SEM_UNDO --> stuck the program\n");
            return;
        }

        semop(semid, &sem_args, 1);
        return;
    }  

    semctl(semid, 0, SETVAL, g_arg);
}

static void IncreaseSemaphore(int semid, const char *const *args)
{
    struct sembuf sem_args;

    /*Set new val*/
    g_arg.val = atoi(args[1]) + GetValSemaphore(semid); 

    /*In case of getting input with undo*/
    if (args[2] && !strcmp(args[2], "undo"))
    {
        sem_args.sem_flg = SEM_UNDO;
        sem_args.sem_op = atoi(args[1]);
        semop(semid, &sem_args, 1);
        return;
    }

    semctl(semid, 0, SETVAL, g_arg);
}

static void ErrorFunc() 
{
    printf("Unsupported command.\n");
    exit(1); 
}

static void PrintValSemaphore(int semid, const char *const *args)
{
    printf("value = %d\n", semctl(semid, 0, GETVAL));
    UNUSED(args);
}   

static void ExitSemaphore()
{
    printf("ExitSemaphore\n");
    exit(0);
}

static int GetValSemaphore(int semid)
{
    return semctl(semid, 0, GETVAL); /*0 = the first sem in set , GetVal = operation cmd*/
}




