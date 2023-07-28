/*******************************************************************************
****** Author: HRD31
****** Date: 02.07.2023
****** Version: 2
*******************************************************************************/
#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h>    /* size_t 		*/
#include <time.h>      /* tmie_t 		*/

#include "uid.h"       /* Uid_t  		*/
#include "scheduler.h" /* scheduler_t   */

typedef struct task task_t;
/*******************************************************************************
Description:     	Creates an empty task.
Return value:    	Pointer to task in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'TaskDestroy()' at end of use.
*******************************************************************************/
task_t *TaskCreate(op_func_t op_func, void *params, size_t interval);


/*******************************************************************************
Description:     	Deletes a task pointed by 'task_t' from memory.
Time Complexity: 	O(n) 
Notes:           	Undefined behaviour if task is NULL.
*******************************************************************************/
void TaskDestroy(task_t *task);


/*******************************************************************************
Description:     	Get Uid for the task.
Return value:    	task 'Uid' for success, 'UID_BAD' for fail.         
Time Complexity: 	O(1).
*******************************************************************************/
Uid_t TaskGetUid(task_t *task);

/*******************************************************************************
Description:     	Execute task to run.
Return value:    	0 for success, 1 for fail (not found).         
Time Complexity: 	O(1)
Notes:              Undefined behaviour if task is invalid pointer.
*******************************************************************************/
int TaskExecute(task_t *task);


/*******************************************************************************
Description:     	
Return value:    	  
Time Complexity: 	
Notes:              
*******************************************************************************/
time_t TaskGetExecuteTime(task_t *task);


/*******************************************************************************
Description:     	
Time Complexity: 	
Notes:              
*******************************************************************************/
void TaskSetExecuteTime(task_t *task, time_t time_current);

/*******************************************************************************
Description:     	
Time Complexity: 	
Notes:              
*******************************************************************************/
size_t TaskGetInterval(task_t *task);

#endif    /*__TASK_H__*/
