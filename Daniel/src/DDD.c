/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog (simple pase)

**************************	Header & Macro	********************************/
#define _POSIX_SOURCE /*include expand library of signal.h (sigation)*/
#include <assert.h>
#include <stdio.h> /*fprintf, scanf*/
#include <stdlib.h> /*system */
#include <unistd.h> /*pid_t*/
#include <pthread.h>
#include <sys/types.h>
#include <signal.h> /*sigaction*/
#include <semaphore.h>	/*	sem_op	*/
#include <string.h>
#include <fcntl.h>/*O_CREAT for sem_op*/
#include <errno.h>

#include "scheduler.h"
#include "aux_funcs.h"


#define UNUSED(x) (void)(x) /*include aux_funcs*/
#define SEM_NAME "/wd_semaphore"

enum 
{
	IS_WD_PROG = 0,
	IS_USER_PROG = 1,
	NUM_wd_args = 3
};


typedef struct wd_args
{
	char **argv_list;
    int argc;
	size_t signal_intervals;
	size_t max_fails;
    int is_user_proc; /*0 user , 1 WdProcess */
    uid_t signal_pid;   
    sem_t *sem; 
} wd_args_t;

/***********************	Function Declerations	**************************/
static void HandleSignal(int signum);
static int SendSignal(void *params);
int WDSchedulerManage(wd_args_t *wd_args_);
int *WatchdogThread();
int MakeMeImurtal(int argc, char *argv[], size_t signal_intervals, size_t max_fails);
int DNR(void);
int SignalMaskHandler(void);

/***************************** Global Definitions *****************************/
static volatile int g_count = 1;
pthread_t tid = 1; 


/***************************    Main    **************************************/

int main(int argc, char *argv[])
{
	int i = 0;
	size_t max_fails = 7;
	size_t signal_intervals = 2;

			/*printf("\n USER APP IS RUNNING|\n");*/
	
	/*Start watchdoog*/
	MakeMeImurtal(argc, argv, signal_intervals, max_fails);
	
	while(i < 100)
	{
		printf("\n USER APP PROGRAM [pid:%d] \n", getpid());
		sleep(1);
	}
	
    pthread_join(tid, NULL);

	/*End watchdoog*/
	DNR();

	return (0);
}

/*User function to initialize the watchdog*/
int MakeMeImurtal(int argc_, char *argv_[], size_t signal_intervals, size_t max_fails) 
{
    int status = 1; 

    status = SignalMaskHandler();
	RETURN_IF_ERROR(0 == status, "SignalMaskHandler error", status);
    
    /*Alocate for struct*/
	wd_args_t *wd_args = (wd_args_t *)malloc(sizeof(wd_args_t));
	RETURN_IF_ERROR(NULL != wd_args, "Failed to allocate memory for wd_args struct!\n", EXIT_FAILURE);

	/*Allocate memory for argv_list (+2 for NULL and program_name)*/ 
	wd_args->argv_list = (char **)(calloc(argc_ + 2, sizeof(char *)));
    RETURN_IF_ERROR(NULL != wd_args->argv_list, "Eror: allocate arg_list", ILRD_FALSE);

    /*Set wd_args*/
    wd_args->argv_list[0] = "./watchdog";
    memcpy(wd_args->argv_list + 1, argv_, argc_ * sizeof(char *));

    wd_args->argc = argc_ + 2;
    wd_args->max_fails = max_fails;
    wd_args->signal_intervals = signal_intervals;
    wd_args->is_user_proc = IS_USER_PROG;
    wd_args->signal_pid = getppid(); 
    /*wd_args->sem = sem_open("SEM_NAME", O_CREAT, 0666, 0);*/

	RETURN_IF_ERROR(SEM_FAILED != wd_args->sem, "sem_open error", ILRD_FALSE);

	/*Create the watchdog thread*/
    status = pthread_create(&tid, NULL, (void *(*)(void *))&WatchdogThread, &wd_args);
    RETURN_IF_ERROR(0 == status, "Eror: creating thread", ILRD_FALSE); 


    /*wait till watchdog ready*/
    sleep(10);
    /*sem_wait(wd_args->sem);
    RETURN_IF_ERROR(-1 != sem_close(wd_args->sem), "sem_close error", ILRD_FALSE);*/

    /*DEBUG*/ printf("MakeMeImurtal after wait\n");


	return 0;
}

int DNR(void)
{
	printf("\n  DNR  \n");
	return (0);
}


int *WatchdogThread(wd_args_t *wd_args_)
{	
    sigset_t signal_set = {0};

	assert(wd_args_);

	/*Unblock SIGUSR1*/
	sigemptyset(&signal_set);
	sigaddset(&signal_set, SIGUSR1);
	pthread_sigmask(SIG_UNBLOCK, &signal_set, NULL);
	
	WDManageSchedulerIMP(info);
	
	/*Clean memory after scheduler done*/
	sem_unlink("PROCESS_IS_READY");
	sem_close(((info_ty *)info)->is_process_ready);
	
	free(((wd_args_t *)wd_args)->argv_for_wd);
	memset(wd_args, 0, sizeof(wd_args_t));
	free((wd_args_t *)wd_args);
	info = NULL;
	
	return (NULL);

    int ststus_sigaction = 0; /*SUCCESS*/
    struct sigaction handler = {0};	
    scheduler_t *sched = NULL;

    /*UnBlock SIGUSR1 and SIGUSR2*/
    sigset_t sigset_to_unblock;
    sigemptyset(&sigset_to_unblock);
    sigaddset(&sigset_to_unblock, SIGUSR1);
    sigaddset(&sigset_to_unblock, SIGUSR2);
    sigprocmask(SIG_UNBLOCK, &sigset_to_unblock, NULL);

    /*Create signal handler for SIGUSR1*/
	handler.sa_handler = &HandleSignal;
    ststus_sigaction = sigaction(SIGUSR1, &handler, NULL);
    ExitIfError(ILRD_SUCCESS == ststus_sigaction, "Eror: sigaction1 failed", ILRD_FALSE); 

    /* Create sched */
    sched = SchedCreate();

    /* Add task to sched */
    SchedAdd(sched, SendSignal, NULL, 1);

    /*Run sched*/
    SchedRun(sched);

    /*CleanUp*/
    SchedDestroy(sched);

    return NULL;   
} 



/*	manages WD scheduler - sends and checks for signals */
/* TODO handle errors for each function in this part */
int WDSchedulerManage(wd_args_t *wd_args_)
{
    int ret_status = 0;
	
	scheduler_t *wd_scheduler = NULL;

	assert(wd_args_);
	
	/*Create scheduler*/
	wd_scheduler = SchedCreate();
	ExitIfError(!wd_scheduler, "Failed to create a WatchDog Scheduler!\n", -1);
	
    /*SchedAdd(wd_scheduler, SendSignal, NULL, 1);*/
	/*ExitIfError(UID_BAD == SchedAdd(wd_scheduler, SendSignal, NULL, 1), "Failed to SchedAdd!\n", -1);*/
	SchedAdd(wd_scheduler, SendSignal, wd_args_, wd_args_->signal_intervals);

	ret_status = SchedRun(wd_scheduler);
	
	SchedDestroy(wd_scheduler);
	
	/*	return if scheduler has successfully finished */
	return (ILRD_SUCCESS == ret_status ? ILRD_SUCCESS : ILRD_FALSE);
}

static void HandleSignal(int signum) 
{
    if (signum == SIGUSR1) 
    {
        g_count = 0;
    }
}

/*SendSignal - not using params*/
static int SendSignal(void *params)
{
    /*send signal*/
    UNUSED(params);
    if (0 == g_count)
    {
        printf(" SendSignal     g_count = 0\n");
        return 0;
    }
    
    printf(" SendSignal     g_count = 1\n");
    return 0;
}

int SignalMaskHandler(void)
{
    sigset_t mask_set;

	/*Block all signals*/
    /*Initialize an empty signal mask_set*/
	RETURN_IF_ERROR(0 == sigemptyset(&mask_set), "sigemptyset error", ILRD_FALSE);
	
	/*Add SIGUSR1 and SIGUSR2 to the mask set*/
	RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR1), "sigaddset error", ILRD_FALSE);
	
	RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR2), "sigaddset error", ILRD_FALSE);
    	
    /*Block mask_set (SIGUSR1, SIGUSR2)*/
    sigprocmask(SIG_BLOCK, &mask_set, NULL);

	return 0; /*mask sucsessfully*/

}