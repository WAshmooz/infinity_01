/*******************************************************************************
****** Author: 		Daniel
****** Reviewer: 	Amit
****** Date: 		14.06.2023
****** Version:		1
*******************************************************************************/
/**********************		Headers		***************************************/

#include <stdlib.h> /* malloc, free, size_t */ 
#include <assert.h> /* assert  */ 
#include "slist.h"
#include "queue.h"

#define LIST(queue) (queue->list)
/******************************************************************************/
/******************************************************************************
description:     	creates an empty queue.
return value:    	pointer to queue in case of success, otherwise NULL.
Time Complexity: 	determined by system call complexity. 
Note:            	should call "QueueDestroy()" at end of use.
*******************************************************************************/
queue_t *QueueCreate(void)
{
	slist_t *slist = NULL;
	queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
	
	if (NULL == queue)
	{
		return NULL;
	}
	
	slist = SListCreate();
	
	if (NULL == slist)
	{
		free(queue);
		queue = NULL;
		
		return NULL;
	}

	LIST(queue) = slist;	

	return queue;
}

/*******************************************************************************
description:     	deletes a queue pointed by "queue" from memory.
Time Complexity: 	determined by system call complexity. 
Notes:           	undefined behaviour if queue is NULL.
*******************************************************************************/
void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);	
	
	SListDestroy(LIST(queue));	
	
	free(queue);
	queue = NULL;
}

/*******************************************************************************
description:     	add 'data' to queue pointed by 'queue'.
return value:    	0 for success, 1 for fail.         
Time Complexity: 	O(1) + system call complexity
Notes:              undefined behaviour if queue is invalid pointer.
*******************************************************************************/
int QueueEnqueue(queue_t *queue, void *data)
{
	assert(NULL != queue);
	
	if (SListInsertBefore(LIST(queue), SListIterEnd(LIST(queue)), data))
	{
		return 0;
	}
	
	return 1;
}


/*******************************************************************************
description:       deletes the first element in "queue".
Time Complexity:   O(1)
Notes:             undefined behaviour if queue is invalid pointer.          
*******************************************************************************/
void QueueDequeue(queue_t *queue)
{
	assert(NULL != queue); 
	
	SlistRemove( SListIterBegin(LIST(queue)) );
}

/*******************************************************************************
description:     	returns number of elements in "queue"
Time Complexity: 	O(n)
Notes:			 	undefined behaviour if queue is invalid pointer
*******************************************************************************/
size_t QueueSize(const queue_t *queue)
{
	assert(NULL != queue); 
		
	return (SListSize(LIST(queue)));
	
	/* its include the dummy as an element */
}

/*******************************************************************************
description:   		returns data of the first element in "queue". 
Time complexity:  	O(1)
Note: 				undefined behaviour if queue is invalid pointer.
*******************************************************************************/
void *QueuePeek(const queue_t *queue)
{
	assert(NULL != queue); 
	
	return (SListGetData( SListIterBegin(LIST(queue))));
}

/*******************************************************************************
description: 		checks if "queue" is empty.
return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				undefined behaviour if queue is invalid pointer.
*******************************************************************************/
int QueueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue); 
	
	return (SListIterBegin(LIST(queue)) == SListIterEnd(LIST(queue)));

}

/*******************************************************************************
description:  	  	Appends "queue2" head to "queue1" tail. 
                    queue2 remains empty and valid.
Time complexity:  	O(1)
Notes:            	undefined behaviour if queues are invalid pointers.
*******************************************************************************/
queue_t *QueueAppend(queue_t *queue1, queue_t *queue2)
{
	assert(NULL != queue1);
	assert(NULL != queue2);
	
	LIST(queue1) = SListAppend(LIST(queue1), LIST(queue2));
	
	return queue1;
}




