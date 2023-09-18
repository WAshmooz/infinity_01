#include <stddef.h>    /* size_t 		*/
#include <time.h>      /* tmie_t 		*/
#include <assert.h>	   /* assert		*/
#include <stdlib.h>	   /* malloc, free  */

#include "scheduler.h" /* scheduler_t   */
#include "uid.h"       /* Uid_t  		*/
#include "task.h"       /* Uid_t  		*/

struct task
{
	Uid_t uid;
	op_func_t op_func;
	void *params;
	size_t interval;
	time_t ex_time;
};

/*******************************************************************************
Description:     	Creates an empty task.
Return value:    	Pointer to task in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'TaskDestroy()' at end of use.
*******************************************************************************/
task_t *TaskCreate(op_func_t op_func, void *params, size_t interval)
{
	task_t *new_task = (task_t *) malloc(sizeof(task_t));
	
	if(NULL == new_task)
	{
		return NULL;
	}
	
	new_task->uid = UidCreate();
	
	if(UidIsEqual(UID_BAD, new_task->uid))
	{
		free(new_task);
		new_task = NULL;
		return NULL;
	}	
	
	new_task->op_func = op_func;	
	new_task->params = params;
	new_task->interval = interval;
	new_task->ex_time = time(NULL) + interval;
	
	if( ((time_t) -1) == new_task->ex_time )
	{
		free(new_task);
		new_task = NULL;
		return NULL;
	}	
	
	
	return new_task;
}

/*******************************************************************************
Description:     	Deletes a task pointed by 'task_t' from memory.
Time Complexity: 	O(n) 
Notes:           	Undefined behaviour if task is NULL.
*******************************************************************************/
void TaskDestroy(task_t *task)
{
	assert(NULL != task);
	
	free(task);
	task = NULL;
}

/*******************************************************************************
Description:     	Get Uid for the task.
Return value:    	task 'Uid' for success, 'UID_BAD' for fail.         
Time Complexity: 	O(1).
*******************************************************************************/
Uid_t TaskGetUid(task_t *task)
{
	assert(NULL != task);
	
	return task->uid;
}

/*******************************************************************************
Description:     	Execute task to run.
Return value:    	0 execute task and repeat
                    1 execute task and remove from scheduler
                    2 (optional)       
Time Complexity: 	O(1)
Notes:              Undefined behaviour if task is invalid pointer.
*******************************************************************************/
int TaskExecute(task_t *task)
{
	assert(NULL != task);
	
	return (task->op_func)(task->params);
}

/*******************************************************************************
Description:     	
Return value:    	  
Time Complexity: 	
Notes:              
*******************************************************************************/
time_t TaskGetExecuteTime(task_t *task)
{
	assert(NULL != task);
	
	return task->ex_time;	
}

/*******************************************************************************
Description:     	
Time Complexity: 	
Notes:              
*******************************************************************************/
void TaskSetExecuteTime(task_t *task, time_t time_current)
{
	assert(NULL != task);
	
	task->ex_time = task->interval + time_current;
}

/*******************************************************************************
Description:     	
Time Complexity: 	
Notes:              
*******************************************************************************/
size_t TaskGetInterval(task_t *task)
{
	assert(NULL != task);
	
	return task->interval;
}
















