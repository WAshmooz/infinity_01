/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			03.09.23 
****Last update:  		11.09.23
****Version:			1
****Description:		Watchdog (common functions)

/**************************Typedef, Stucts and Enums***************************/
enum 
{
	ARR_ARGV_SIZE = 20,
	ARR_FNAME_SIZE = 100,
	ARR_NUM_SIZE = 32
};

/******************************Global variables********************************/
static volatile int g_fail_counter;
static volatile int g_is_not_resucitate;
static volatile int g_is_child_ready;

/****************************Function prototypes*******************************/
int BlockAllSignalsHandler(void); /*Block all signals*/
int BlockSignalHandler(void);   /*Block SIGUSR1, SIGUSR2*/
int UnBlockSignalHandler(void); /*UnBlock SIGUSR1, SIGUSR2*/


/*
static void SIGUSR1Handler(int signo_, siginfo_t *info_, void *context_);
static void SIGUSR2Handler(int signo_);
static int Resucitate(wd_args_t *args_);
static int SigSenderAndCountChecker(wd_args_t *args_);
static int FirstSignal(wd_args_t *args_);
static void InitArgs(wd_args_t *args_, char **arr_args_, char *arr_freq_, 
																char *arr_fc_);*/
/**********************************Functions***********************************/
int WDSchedulerManage(wd_args_t *wd_args_)
{
    int ret_status = 0;
    scheduler_t *wd_scheduler = NULL;

    printf("\n\nWDSchedulerManage START Thread\n\n");

    /*Create scheduler*/
    wd_scheduler = SchedCreate();
    ExitIfError(NULL != wd_scheduler, "Failed to create a WatchDog Scheduler!\n", -1);

    /*SchedAdd(wd_scheduler, SendSignal, NULL, 1);*/
    /*ExitIfError(UID_BAD == SchedAdd(wd_scheduler, SendSignal, NULL, 1), "Failed to SchedAdd!\n", -1);*/
    SchedAdd(wd_scheduler, StamScheduler, wd_args_, wd_args_->signal_intervals);

    ret_status = SchedRun(wd_scheduler);

    SchedDestroy(wd_scheduler);

    /*	return if scheduler has successfully finished */
    return (ILRD_SUCCESS == ret_status ? ILRD_SUCCESS : ILRD_FALSE);
}

int BlockAllSignalsHandler(void) 
{
    sigset_t all_signals;
    
    /*Initialize the set to include all possible signals*/
    if (sigfillset(&all_signals) != 0) 
     RETURN_IF_ERROR(sigfillset(&all_signals) != 0, "sigprocmask error", ILRD_FALSE);

    /*Block all signals*/
    if (sigprocmask(SIG_BLOCK, &all_signals, NULL) != 0) 
    RETURN_IF_ERROR(sigprocmask(SIG_BLOCK, &all_signals, NULL) != 0,
                                                    "sigprocmask error", ILRD_FALSE);
    
    return 0; /*BlockAll sucsessfully*/
}

int BlockSignalHandler(void)
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

    return 0; /*Block sucsessfully*/

}

int UnBlockSignalHandler(void)
{
    sigset_t mask_set;

    /*Block all signals*/
    /*Initialize an empty signal mask_set*/
    RETURN_IF_ERROR(0 == sigemptyset(&mask_set), "sigemptyset error", ILRD_FALSE);

    /*Add SIGUSR1 and SIGUSR2 to the mask set*/
    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR1), "sigaddset error", ILRD_FALSE);

    RETURN_IF_ERROR(0 == sigaddset(&mask_set, SIGUSR2), "sigaddset error", ILRD_FALSE);
        
    /*Block mask_set (SIGUSR1, SIGUSR2)*/
    sigprocmask(SIG_UNBLOCK, &mask_set, NULL);

    return 0; /*UnBlock sucsessfully*/
}