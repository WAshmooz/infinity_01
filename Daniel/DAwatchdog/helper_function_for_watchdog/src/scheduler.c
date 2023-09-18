/*******************************************************************************
****** Author: HRD31
****** Date: 02.07.2023
****** Version: 2
*******************************************************************************/

#include <stdlib.h>  /* malloc, free, size_t */
#include <assert.h>  /* assert */
#include <unistd.h>  /* sleep */

#include "uid.h"     /* Uid_t  */
#include "p_queue.h" 
#include "task.h" 
#include "scheduler.h" 

#ifndef NDEGUB
#include <stdio.h>
#endif

#define S_QUEUE sched->queue
#define S_IS_RUNNING sched->is_runnig
#define S_IS_SELF_REMOVE sched->is_self_remove
#define S_RUNNING_TASK sched->runnig_task

/*SchedRemove*/
#define SUCCESS 0
#define FAIL 	1

/*isEmpty*/
#define EMPTY		1
#define NOT_EMPTY 	0

/*op_func*/
#define EXECUTE_REPEAT 	0
#define EXECUTE_REMOVE	1
#define OPTIONAL 		2

/*is_runnig*/
#define OFF 0
#define ON 	1

/*UidIsEqual*/
#define FALSE 0
#define TRUE 	1

struct scheduler
{
	p_queue_t *queue;
	int is_runnig;
	int is_self_remove;
	task_t *runnig_task;
};
/*******************************************************************************
Description:     	Creates an empty scheduler.
Return value:    	Pointer to scheduler in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'SchedulerDestroy()' at end of use.
*******************************************************************************/
static int SortIntervalfunc(const void *new_task, const void *cmp_task)
{
	task_t *new_ex_time = (task_t *) new_task;
	task_t *cmp_ex_time = (task_t *) cmp_task;
	
	return ( TaskGetExecuteTime(new_ex_time) - TaskGetExecuteTime(cmp_ex_time) );	
}

scheduler_t *SchedCreate(void)
{
	scheduler_t *sched = (scheduler_t *) malloc(sizeof(scheduler_t));
	
	if (NULL == sched)
	{
		return NULL;
	}
	
	S_QUEUE = PQueueCreate(SortIntervalfunc);
	
	if (NULL == S_QUEUE)
	{
		free(sched);
		sched = NULL;
		return NULL;
	}
	
	S_IS_RUNNING = OFF;
	S_IS_SELF_REMOVE = OFF;
	S_RUNNING_TASK = NULL;

	return sched;
}

/*******************************************************************************
Description:     	Deletes a scheduler pointed by 'scheduler_t' from memory.
Time Complexity: 	O(n) 
Notes:           	Undefined behaviour if scheduler is NULL.
*******************************************************************************/
void SchedDestroy(scheduler_t *sched)
{
	assert(NULL != sched);
	
	SchedClear(sched);
	PQueueDestroy(S_QUEUE);
	free(sched);
	sched = NULL;	
}

/*******************************************************************************
Description:     	Add 'task' to scheduler pointed by 'scheduler'.
Return value:    	task 'Uid' for success, 'UID_BAD' for fail.         
Time Complexity: 	O(n)
Notes:              interval in seconds
                    Undefined behaviour if 'scheduler' is invalid pointer.
                    Undefined behaviour if 'op_func' is invalid pointer.
*******************************************************************************/
Uid_t SchedAdd( scheduler_t *sched,
    				op_func_t op_func,
   					void *params,
    				size_t interval)
{
	task_t *new_task = TaskCreate(op_func, params, interval);
		
	assert(NULL != sched);
	
	if(NULL == new_task)
	{
		return UID_BAD;
	}

	if( FAIL == PQueueEnqueue(S_QUEUE, new_task) )
	{
		TaskDestroy(new_task);
		new_task = NULL;
		return UID_BAD;
	}

	return TaskGetUid(new_task);
}

/*******************************************************************************
Description:     	Remove task with 'task_uid_to_remove' from scheduler pointed
                    by 'scheduler'.
Return value:    	0 for success, 1 for fail (not found).         
Time Complexity: 	O(n)
Notes:              Undefined behaviour if scheduler is invalid pointer.
*******************************************************************************/
typedef struct uid_wrap
{
	Uid_t uid;
	task_t *task_to_remove;	
}uid_wrap_t;

int IsMatchUid(const void *task_from_pq, const void *uid_wrap_from_user)
{
	task_t *task_pq = (task_t *)task_from_pq;
	uid_wrap_t *uid_wrap_user = (uid_wrap_t *)uid_wrap_from_user;
	if( TRUE == UidIsEqual(TaskGetUid(task_pq), uid_wrap_user->uid) )
	{
		uid_wrap_user->task_to_remove = task_pq;
		return 1;
	}
	
	return 0;
}

int SchedRemove(scheduler_t *sched, Uid_t uid_to_remove)
{
	int status = 1;
	/*variable of uid_wrap_t + updating its Uid*/
	uid_wrap_t uid_wrap_user;
	uid_wrap_user.uid = uid_to_remove;

	assert(NULL != sched);

	/* flag SELF_REMOVE is ON in case of identical Uid (runing_uid = remove_uid)*/
	if (ON == S_IS_RUNNING && TRUE == UidIsEqual(TaskGetUid(S_RUNNING_TASK), uid_to_remove) )
	{
		S_IS_SELF_REMOVE = ON;
	}
	
	status = PQueueErase(S_QUEUE, IsMatchUid, &uid_wrap_user);

	if (!status)
	{
		TaskDestroy(uid_wrap_user.task_to_remove);
		return 0;
	}

	return 1;
}

/*******************************************************************************
Description:     	Run all tasks in scheduler pointed by 'scheduler'.
Return value:    	0 for success, 1 for fail (not found).     <<<   ( still not final)   
Time Complexity: 	Depends Tasks to be executed
Notes:              Undefined behaviour if scheduler is invalid pointer.
*******************************************************************************/
static void SleepFun(unsigned int seconds) 
{
	unsigned int left_seconds = seconds;
	
	while(0 < left_seconds)
	{
		left_seconds = sleep(left_seconds);	
	}
}


int SchedRun(scheduler_t *sched)
{
	time_t current_time;
	time_t sleep_time;
																	
	assert(NULL != sched);
																		
	S_IS_RUNNING = ON;
																		
	while(EMPTY != PQueueIsEmpty(S_QUEUE) && ON == S_IS_RUNNING)
	{																	
		/*initialize current time*/
		current_time = time(NULL);
		
		if( ((time_t) -1) == current_time )
		{
			return FAIL;
		}
		
		/*initialize sleep_time and call the SleepFun*/
		sleep_time = TaskGetExecuteTime( PQueuePeek(S_QUEUE) );
		sleep_time = (sleep_time > current_time) ? (sleep_time - current_time) : 0;	 
		SleepFun(sleep_time);
		
		/*initialize the pointer to the runing task*/
		S_RUNNING_TASK = (task_t *)PQueuePeek(S_QUEUE);
		
		/*runing time*/
		PQueueDequeue(S_QUEUE);

        switch (TaskExecute(S_RUNNING_TASK))
        {
            case EXECUTE_REPEAT:
                
                current_time = time(NULL);
                
                if (((time_t)-1) == current_time)
                {
                    return FAIL;
                }    
                

                if (ON == S_IS_SELF_REMOVE)
                {    	   
              	 TaskSetExecuteTime(S_RUNNING_TASK, current_time);	
                break;
                              	 				
                } 
                
                if (OFF == S_IS_SELF_REMOVE)
                {
                PQueueEnqueue(S_QUEUE, S_RUNNING_TASK);
				TaskSetExecuteTime(S_RUNNING_TASK, current_time);
                }
                
                break;
  				

            case EXECUTE_REMOVE:

            	TaskDestroy(S_RUNNING_TASK);
            
                break;
              
            case OPTIONAL:
            
            	TaskDestroy(S_RUNNING_TASK);           
                return FAIL;
                
                break;
        }
    }
	
	return SUCCESS;
}
	/*	update "timr_t time_current" to time(NULL)	*/ 
/*******************************************************************************
Description:     	stops the run of all tasks in scheduler pointed by
                    'scheduler'.
Time Complexity: 	O(1)
Notes:              Can be used as an operation function inside tasks to stop
                    the running of the scheduler.
                    Undefined behaviour if scheduler is invalid pointer.
*******************************************************************************/
void SchedStop(scheduler_t *sched)
{
	S_IS_RUNNING = OFF;  	
}

/*******************************************************************************
Description:     	Returns number of elements in 'scheduler'
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if scheduler is invalid pointer
*******************************************************************************/
size_t SchedSize(const scheduler_t *sched)
{
	assert(NULL != sched);
	
	return PQueueSize(S_QUEUE);		
}

/*******************************************************************************
Description: 		Checks if 'scheduler' is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if scheduler is invalid pointer.
*******************************************************************************/
int SchedIsEmpty(const scheduler_t *sched)
{
	assert(NULL != sched);
	
	return PQueueIsEmpty(S_QUEUE);			
}

/*******************************************************************************
Description: 		remove all tasks from 'scheduler'.
Time complexity:  	O(n)
Note: 				Undefined behaviour if scheduler is invalid pointer.
*******************************************************************************/
void SchedClear(scheduler_t *sched)
{
	assert(NULL != sched);
	
	while(! PQueueIsEmpty(S_QUEUE))
	{ 
		TaskDestroy(PQueuePeek(S_QUEUE));
		PQueueDequeue(S_QUEUE);
	}		
}




