/*******************************************************************************
****** Author: HRD31
****** Date: 02.07.2023
****** Version: 1
*******************************************************************************/

#include <stdlib.h>         /* malloc, free, size_t */
#include <assert.h>         /* assert        		*/

#include "sorted_list.h" 	/* srtitr_t */
#include "p_queue.h"

#define PqToSrt(p_queue_t) (p_queue->list)	/*	p_queue_t to srtlist_t	*/

#define SUCCESS 0
#define FAIL    1

struct p_queue
{
	srtlist_t *list;
};


/*******************************************************************************
Description:     	Creates an empty p_queue sorted by 'priority_func'.
Return value:    	Pointer to p_queue in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'PQueueDestroy()' at end of use.
*******************************************************************************/
p_queue_t *PQueueCreate(p_sort_func_t priority_func)
{
	p_queue_t *p_queue = (p_queue_t *)malloc(sizeof(p_queue_t));
	
	/* if allocation fail for p_queue */ 
	if(NULL == p_queue)
	{
		return NULL;
	}
	
	p_queue->list = SrtListCreate(priority_func);
	
	/* if allocation fail for SrtListCreate we need to free p_queue */ 
	if(NULL == PqToSrt(p_queue))
	{
		free(p_queue);
		p_queue = NULL;
		return NULL;
	}
	
	return p_queue;
}

/*******************************************************************************
Description:     	Deletes a p_queue pointed by 'p_queue' from memory.
Time Complexity: 	O(n) 
Notes:           	Undefined behaviour if p_queue is NULL.
*******************************************************************************/
void PQueueDestroy(p_queue_t *p_queue)
{ 
	assert(NULL != p_queue);
	
	SrtListDestroy(PqToSrt(p_queue));
	free(p_queue);
	p_queue = NULL;	
}
/*******************************************************************************
Description:     	Add 'data' to p_queue pointed by 'p_queue' 
					according to priority provided by user.
Return value:    	0 for success, 1 for fail.         
Time Complexity: 	O(n)
Notes:              Undefined behaviour if p_queue is invalid pointer.
*******************************************************************************/
int PQueueEnqueue(p_queue_t *p_queue, void *data)
{
	assert(NULL != p_queue);	
		
	return SrtListIterIsEqual(SrtListInsert(PqToSrt(p_queue), data),
		                      SrtListIterEnd(PqToSrt(p_queue)) );
}

/*******************************************************************************
Description:       Deletes the first element in 'p_queue'.
Time Complexity:   O(1)
Notes:             Undefined behaviour if p_queue is invalid pointer.          
*******************************************************************************/
void PQueueDequeue(p_queue_t *p_queue)
{
	assert(NULL != p_queue); 
	
	SrtListPopBack(PqToSrt(p_queue));
}

/*******************************************************************************
Description:   		Returns data of the first element in 'p_queue'. 
Time complexity:  	O(1)
Note: 				Undefined behaviour if p_queue is invalid pointer.
*******************************************************************************/
void *PQueuePeek(const p_queue_t *p_queue)
{
	assert(NULL != p_queue); 
	
	return SrtListGetData(SrtListIterPrev(SrtListIterEnd(p_queue->list)));
}

/*******************************************************************************
Description: 		Checks if 'p_queue' is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if p_queue is invalid pointer.
*******************************************************************************/
int PQueueIsEmpty(const p_queue_t *p_queue)
{
	assert(NULL != PqToSrt(p_queue));	
	
	return SrtListIsEmpty(PqToSrt(p_queue));		
}

/*******************************************************************************
Description:     	Returns number of elements in 'p_queue'
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if p_queue is invalid pointer
*********************************************************************************/
size_t PQueueSize(const p_queue_t *p_queue)
{
	assert(NULL != p_queue->list);	
	
	return SrtListSize(PqToSrt(p_queue));
}

/*******************************************************************************
Description:  	  	Clears all elements in a priority queue pointed by 'p_queue'
					from memory. 
Time complexity:  	O(n)
Notes:            	Undefined behaviour if 'p_queue' is invalid pointers.
*******************************************************************************/
void PQueueClear(p_queue_t *p_queue)
{	
	assert(NULL != (PqToSrt(p_queue)));	
	
	while(!(PQueueIsEmpty(p_queue)))
	{
		PQueueDequeue(p_queue);
	} 
}

/*******************************************************************************
Description:  	  	Erase the first element with 'data' found by 'match_func'
                    from a priority queue pointed by 'p_queue' from memory.
Return value 		0 for success otherwise 1.
Time complexity:  	O(n)
Notes:            	Undefined behaviour if 'p_queue' is invalid pointer.
*******************************************************************************/
int PQueueErase(p_queue_t *p_queue, is_match_func_t match_func, void *data)
{
	srtitr_t srt_itr; 
	srtitr_t from = SrtListIterBegin(PqToSrt(p_queue));
	srtitr_t to = SrtListIterEnd(PqToSrt(p_queue));
	
	assert(NULL != p_queue);	
	
	srt_itr = SrtListFindIf(from, to, match_func, data);
	
	if (SrtListIterIsEqual(srt_itr, SrtListIterEnd(PqToSrt(p_queue))))
		{
			return FAIL;
		}			
	SrtListRemove(srt_itr);
	 
	return SUCCESS;	
			
}


