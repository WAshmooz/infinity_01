/*******************************************************************************
****** Author: HRD31
****** Date: 02.07.2023
****** Version: 1
*******************************************************************************/
#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#include <stddef.h>  		/* size_t 	        */
#include "aux_funcs.h"      /* is_match_func_t 	*/

typedef struct p_queue p_queue_t;

/******************************************************************************
Description:     	Performs sort function for sorting by priority.
Return value:    	Returns 0 if new_data == data, Positive number if 
                    new_data > data, Negative number if new_data < data
*******************************************************************************/
typedef int (*p_sort_func_t) (const void *new_data, const void *data);


/*******************************************************************************
Description:     	Creates an empty p_queue sorted by 'priority_func'.
Return value:    	Pointer to p_queue in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'PQueueDestroy()' at end of use.
*******************************************************************************/
p_queue_t *PQueueCreate(p_sort_func_t priority_func);


/*******************************************************************************
Description:     	Deletes a p_queue pointed by 'p_queue' from memory.
Time Complexity: 	O(n) 
Notes:           	Undefined behaviour if p_queue is NULL.
*******************************************************************************/
void PQueueDestroy(p_queue_t *p_queue);


/*******************************************************************************
Description:     	Add 'data' to p_queue pointed by 'p_queue' according to 
					priority provided by user.
Return value:    	0 for success, 1 for fail.         
Time Complexity: 	O(n)
Notes:              Undefined behaviour if p_queue is invalid pointer.
*******************************************************************************/
int PQueueEnqueue(p_queue_t *p_queue, void *data);


/*******************************************************************************
Description:       Deletes the first element in 'p_queue'.
Time Complexity:   O(1)
Notes:             Undefined behaviour if p_queue is invalid pointer.          
*******************************************************************************/
void PQueueDequeue(p_queue_t *p_queue);


/*******************************************************************************
Description:   		Returns data of the first element in 'p_queue'. 
Time complexity:  	O(1)
Note: 				Undefined behaviour if p_queue is invalid pointer.
*******************************************************************************/
void *PQueuePeek(const p_queue_t *p_queue);


/*******************************************************************************
Description: 		Checks if 'p_queue' is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if p_queue is invalid pointer.
*******************************************************************************/
int PQueueIsEmpty(const p_queue_t *p_queue);


/*******************************************************************************
Description:     	Returns number of elements in 'p_queue'
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if p_queue is invalid pointer
*******************************************************************************/
size_t PQueueSize(const p_queue_t *p_queue);

/*******************************************************************************
Description:  	  	Clears all elements in a priority queue pointed by 'p_queue'
					from memory. 
Time complexity:  	O(n)
Notes:            	Undefined behaviour if 'p_queue' is invalid pointers.
*******************************************************************************/
void PQueueClear(p_queue_t *p_queue);


/*******************************************************************************
Description:  	  	Erase the first element with 'data' found by 'match_func'
                    from a priority queue pointed by 'p_queue' from memory.
Return value 		0 for success otherwise 1.
Time complexity:  	O(n)
Notes:            	Undefined behaviour if 'p_queue' is invalid pointer.
*******************************************************************************/
int PQueueErase(p_queue_t *p_queue, is_match_func_t match_func, void *data);

#endif    /*__PQUEUE_H__*/
