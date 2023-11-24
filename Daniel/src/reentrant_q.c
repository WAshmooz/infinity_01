/*******************************************************************************
*  Author:                                           
*  Reviewer:                             
*  Date: 27/08/23                                            
*  Version:
*  Description:      
*******************************************************************************/
#ifndef _OPEN_THREADS                                                           
#define _OPEN_THREADS                                                           
#endif  

#include <assert.h>		/* assert 											*/
#include <stdio.h> 		/* printf, puts										*/
#include <stddef.h>		/* 	offsetof										*/
#include <stdlib.h>		/* malloc, free,									*/
#include <pthread.h>	/* pthread_create, pthread_exit, pthread_t, 
						pthread_mutex_t, pthread_mutex_init, 
						pthread_mutex_destroy								*/
#include <semaphore.h>	/* sem_t, sem_init, sem_destroy 					*/

#include "aux_funcs.h"	/* RETURN_IF_ERROR, ExitIfError						*/
#include "reentrant_q.h"
/*********************************Macros***************************************/

/**************************Typedef, Stucts and Enums***************************/
enum 
{
	LOCK_FAIL = 1,
	UNLOCK_FAIL,
	MUTEX_DESTROY_FAIL,
	SEM_DESTROY_FAIL,
	SEM_POST_FAIL
};

struct fixed_size_queue
{
	size_t capacity;
	size_t elem_size;
	pthread_mutex_t mutex_enq;
	pthread_mutex_t mutex_deq;
	char *read;
	char *write;
	sem_t sem_enq;
	sem_t sem_deq;
	char arr[1];
};


/****************************Function prototypes*******************************/

/*
int main(void) 
{	
	return 0;
}
*/
/********************************Pseudo code***********************************/

			
/**********************************Functions***********************************/
fsq_t *FsqCreate(size_t capacity_, size_t elem_size_)
{
	fsq_t *queue = NULL;
	pthread_mutex_t mutex1, mutex2;
	sem_t sem1, sem2;
	size_t queue_size = 0;

	assert(0 != capacity_);
	assert(0 != elem_size_);
	
	queue_size = offsetof(fsq_t, arr) + (capacity_ + 1) * sizeof(char);
	
	/*allocate memory for the fixed size queue and check if it has been 
	successful*/
	queue = (fsq_t *)malloc(queue_size);
	RETURN_IF_ERROR(NULL != queue, "malloc error", NULL);
	
	/*init mutexes that will prevent entry to the critical section for several 
	producer threads at the same time*/
	RETURN_IF_ERROR(0 == pthread_mutex_init(&mutex1, NULL), "mutex_init error", 
																		NULL);
	
	/*init mutex with the same purpose but for consumer threads*/
	RETURN_IF_ERROR(0 == pthread_mutex_init(&mutex2, NULL), "mutex_init error", 
																		NULL);
																		
	/*init enqueue semaphore with a value equal to the number of elements that
	 can be added to the queue*/
	RETURN_IF_ERROR(0 == sem_init(&sem1, 0, capacity_/elem_size_), 
														"sem_init error", NULL);
	
	/*init dequeue semaphore with a value of zero equal which is an indicator
	that no element can be dequeued*/
	RETURN_IF_ERROR(0 == sem_init(&sem2, 0, 0), "sem_init error", NULL);
	
	queue->capacity = capacity_;
	queue->elem_size = elem_size_;
	queue->mutex_enq = mutex1;
	queue->mutex_deq = mutex2;
	queue->read = queue->arr;
	queue->write = queue->arr;
	queue->sem_enq = sem1;
	queue->sem_deq = sem2;
	
	return queue;
}


void FsqDestroy(fsq_t *queue_)
{
	assert(NULL != queue_);
	
	ExitIfError(0 == pthread_mutex_destroy(&queue_->mutex_enq), 
									"mutex_destroy error", MUTEX_DESTROY_FAIL);
	
	ExitIfError(0 == pthread_mutex_destroy(&queue_->mutex_deq), 
									"mutex_destroy error", MUTEX_DESTROY_FAIL); 
	
	ExitIfError(0 == sem_destroy(&queue_->sem_enq), 
										"sem_destroy error", SEM_DESTROY_FAIL);
	ExitIfError(0 == sem_destroy(&queue_->sem_deq), 
										"sem_destroy error", SEM_DESTROY_FAIL); 
	
	free(queue_); queue_ = NULL;
}


void FsqEnqueue(fsq_t *queue_, void *data)
{
	/*check if the value of the semaphore is greater than 0, if so, it indicates
	that the queue is not full and it's possible to enqueue a new element to it 
	and decrease the semaphore value by one*/
	if(0 == sem_wait(&queue_->sem_enq))
	{
		/*close the mutex so that only one of the threads can access the
		 critical section*/	
		ExitIfError(0 == pthread_mutex_lock(&queue_->mutex_enq), 
											"mutex_lock() error", LOCK_FAIL);        
			
		
		/*close the mutex*/
		ExitIfError(0 == pthread_mutex_unlock(&queue_->mutex_enq), 
										"mutex_unlock() error", UNLOCK_FAIL);
		
	/*increment the value of the semaphore each time enqueue a new element to 
	the queue*/ 
	ExitIfError(0 == sem_post(&queue_->sem_deq), "sem_post error", SEM_POST_FAIL);		 
	}
}

/* TODO for all functions that block check errno */
void FsqDequeue(fsq_t *queue_, void *buffer)
{
	/*check if the value of the semaphore is greater than 0, if so, it indicates
	 that there is at least one element that can be dequeued and decrease the 
	 semaphore value by one*/
	if(0 == sem_wait(&queue_->sem_deq))
	{
		ExitIfError(0 == pthread_mutex_lock(&queue_->mutex_deq), 
											"mutex_lock() error", LOCK_FAIL); 
											
		
		ExitIfError(0 == pthread_mutex_unlock(&queue_->mutex_deq), 
										"mutex_unlock() error", UNLOCK_FAIL); 
	
	/*increment the value of the semaphore each time dequeue a last element from
	the queue*/
	ExitIfError(0 == sem_post(&queue_->sem_enq), "sem_post error", SEM_POST_FAIL);		
	}
}

