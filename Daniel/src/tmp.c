/***************************************************************************
****Author: 			Daniel Shabso
****Reviewer: 			
****Creation: 			24.08.23 
****Last update:  		24.08.23
****Version:			1
****Description:		Prooducers-Consumers  

**************************	Header & Macro	********************************/
#include <stdio.h>		/*printf*/
#include <pthread.h>
#include <stdatomic.h>



enum {BUFFER_SIZE = 10} ;

/***************************	Prototypes	********************************/
static void produce(int data);
static int consume();
static void* producer(void* arg); 
static void* consumer(void* arg); 

/********************************	Main	********************************/
atomic_int buffer[BUFFER_SIZE];
atomic_flag lock = ATOMIC_FLAG_INIT;

static void produce(int data) 
{
	/*Wait until the lock is acquired*/
	while (atomic_flag_test_and_set(&lock)) {}

	/*Produce data and put it in the buffer*/
	for (int i = 0; i < BUFFER_SIZE; i++) 
	{
		if (atomic_load(&buffer[i]) == 0) 
		{
			atomic_store(&buffer[i], data);
			break;
		}
	}

	/*Release the lock*/
	atomic_flag_clear(&lock);
}

static int consume() 
{
	int data = 0;

	/*Wait until the lock is acquired*/
	while (atomic_flag_test_and_set(&lock)) {}

	/*Consume data from the buffer*/
	for (int i = 0; i < BUFFER_SIZE; i++) 
	{
		if (atomic_load(&buffer[i]) != 0) 
		{
			data = atomic_exchange(&buffer[i], 0);
			break;
		}
	}

	/*Release the lock*/
	atomic_flag_clear(&lock);

	return data;
}

static void* producer(void* arg) 
{
	for (int i = 1; i <= 10; i++) 
	{
		produce(i);
		printf("Produced: %d\n", i);
	}
	return NULL;
}

static void* consumer(void* arg) 
{
	for (int i = 1; i <= 10; i++) 
	{
		int data = consume();
		printf("Consumed: %d\n", data);
	}
	return NULL;
}

int main() 
{
	pthread_t producer_thread, consumer_thread;

	/*Initialize buffer with zeros*/
	for (int i = 0; i < BUFFER_SIZE; i++) 
	{
		atomic_init(&buffer[i], 0);
	}

	/*Create producer and consumer threads*/
	pthread_create(&producer_thread, NULL, producer, NULL);
	pthread_create(&consumer_thread, NULL, consumer, NULL);

	/*Wait for threads to finish*/
	pthread_join(producer_thread, NULL);
	pthread_join(consumer_thread, NULL);

	return 0;
}

    
    
    
    

    
    
    
    
    
