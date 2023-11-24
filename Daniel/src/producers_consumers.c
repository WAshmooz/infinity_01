/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer:  
****** Date: 28.08.23
****** Version: 1 
****** Description: Producers-Consumers 
*******************************************************************************/
#include <stddef.h>    /* size_t, NULL */
#include <assert.h>	   /* assert */
#include <pthread.h>   /* pthread_... */
#include <stdlib.h>    /* srand, rand */
#include <time.h>      /* time */
#include <stdio.h>     /* printf */
#include <unistd.h>    /* sleep, ssize_t */
#include <semaphore.h> /* sem... */

#include "aux_funcs.h" /* ExitIfError, UNUSED */
#include "slist.h"	   /* Slist... */
#include "cbuffer.h"   /* CBuffer... */

enum {THREAD_NUM=10, QUEUE_SIZE=100, CONSUM_NUM=3};

/*******************************************************************************
 								Structs & Typedefs
*******************************************************************************/
/* for exercise 1 */
typedef struct 
{
	volatile int spinlock;
	int random_int;
} exe1_args_t;

/* for exercise 2 */
typedef struct 
{
	slist_t *list;
	pthread_mutex_t mutex;
} exe2_args_t;

/* for exercise 3 */
typedef struct 
{
	slist_t *list;
	pthread_mutex_t mutex;
	sem_t semaphore;
} exe3_args_t;

/* for exercise 4 */
typedef cbuffer_t fsq_t;
typedef struct 
{
	fsq_t *queue;
	pthread_mutex_t mutex;
	sem_t in_semaph;
	sem_t out_semaph;
} exe4_args_t;

typedef struct 
{
	volatile int msg;
	volatile size_t consum_count;
	volatile size_t read_count;
	pthread_mutex_t mutex;
	sem_t semaphor;
	pthread_cond_t read_var;
} exe6_args_t;


/*******************************************************************************
							Functions Declarations 
*******************************************************************************/
static void Exercise1(void);
static void *Producer1(void *args_);
static void *Consumer1(void *args_);
static void Exercise2(void);
static void *Producer2(void *args_);
static void *Consumer2(void *args_);
static void Exercise3(void);
static void *Producer3(exe3_args_t *args_);
static void *Consumer3(exe3_args_t *args_);
static void Exercise4(void);
static void *Producer4(exe4_args_t *args_);
static void *Consumer4(exe4_args_t *args_);
static void Exercise6(void);
static void *Producer6(exe6_args_t *args_);
static void *Consumer6(exe6_args_t *args_);

/*******************************************************************************
						Helper Functions Declarations
*******************************************************************************/
static int ProduceRandInt(void);
static void PrintInt(int num_);

/* FSQ API */
static fsq_t *FSQcreate(size_t q_size_);
static void FSQdestroy(fsq_t * fsq_);
static ssize_t FSQenqueue(fsq_t *fsq_, const int *src_);
static ssize_t FSQdequeue(fsq_t *fsq_, int *dest_);

/*******************************************************************************
								   Functions 
*******************************************************************************/
int main(void)
{
	srand(time(NULL));

	UNUSED(Exercise1);
	UNUSED(Exercise2);
	UNUSED(Exercise3);
	UNUSED(Exercise4);

	Exercise6();


	return 0;
}


/*******************************************************************************
								   Exercise 6
*******************************************************************************/
static void Exercise6(void)
{
	size_t i = 0;
	pthread_t threads_id[CONSUM_NUM + 1] = {0};

	/*create argument struct, initialize mutex & cond_var */
	exe6_args_t args = {0, 0, 0, PTHREAD_MUTEX_INITIALIZER, {{0}}, 
				    	PTHREAD_COND_INITIALIZER};

	/* create semaphores */
	ExitIfError(0 == sem_init(&args.semaphor, 0, 0),
	"init semaphore failed\n", 8);

	/* create 1 producer and THREAD_NUM consumers */
	ExitIfError(0 == pthread_create(&threads_id[0], NULL, 
			(void *(*)(void *))&Producer6,(void *)(&args)),
			"pthread create producer failed\n", 1);
	
	for (i = 1; i < CONSUM_NUM + 1; ++i)
	{

		ExitIfError(0 == pthread_create(&threads_id[i], NULL, 
				   (void *(*)(void *))&Consumer6, (void *)(&args)), 
				   "pthread create consumer failed\n", 1);
	}

	/* TODO - delete this part */
	{
		sleep(5);
		return;
	}

	/* join threads */
	for (i = 0; i < CONSUM_NUM + 1; ++i)
	{
		ExitIfError(0 == pthread_join(threads_id[i], NULL), "join failed\n", 2);
	}

	/* destroy semaphore and cond_var */
	ExitIfError(0 == sem_destroy(&args.semaphor), "sem_destroy failed\n", 13);
	ExitIfError(0 == pthread_cond_destroy(&args.read_var),
	"cond_destroy failed\n", 15);
}

static void *Producer6(exe6_args_t *args_)
{	
	int local_rand = 0;
	int i = 0;

	while (1)
	{	
		/* produce random number */
		local_rand = ProduceRandInt();
		
		/* lock mutex */
		ExitIfError(0 == pthread_mutex_lock(&args_->mutex), 
					"lock mutex failed\n", 5);

		/* write message */
		args_->msg = local_rand;
		
		/* set condition variable to zero */
		args_->consum_count = 0;

		/* unlock semaphore */
		for (i = 0; i < CONSUM_NUM; ++i)
		{
			ExitIfError(0 == sem_post(&args_->semaphor), "sem_post failed\n", 7);
		}

		/* wait for condition to change and signal */
		while(args_->consum_count < CONSUM_NUM)
		{
			pthread_cond_wait(&args_->read_var, &args_->mutex);
			/*TODO - check failed*/
		}

		/* unlock mutex */
		ExitIfError(0 == pthread_mutex_unlock(&args_->mutex),
		"unlock mutex failed\n", 5);
	}

	return NULL;
}


static void *Consumer6(exe6_args_t *args_)
{
	int local_rand = 0;
	
	while (1)
	{	
		/* lock semaphore */
		ExitIfError(0 == sem_wait(&args_->semaphor), "sem_wait failed\n", 9);

		/* read message */
		local_rand = args_->msg;

		/* lock mutex */
		ExitIfError(0 == pthread_mutex_lock(&args_->mutex), 
					"lock mutex failed\n", 5);

		/* update read counter */
		++args_->read_count;

		/* last consumer send brodcast to all consumers */
		if (args_->read_count == CONSUM_NUM)
		{
			pthread_cond_broadcast(&args_->read_var);
		}

		/* all the consumers exepet the last waits */
		while(args_->read_count < CONSUM_NUM)
		{
			pthread_cond_wait(&args_->read_var, &args_->mutex);
			/*TODO - check failed*/
		}

		/* update consumers counter */
		++args_->consum_count;

		/* last consumer out of the critical section send a signal to the
		 producer and reset the reade counter*/
		if (args_->consum_count == CONSUM_NUM)
		{
			pthread_cond_signal(&args_->read_var);
			args_->read_count = 0;
		}

		/* unlock mutex */
		ExitIfError(0 == pthread_mutex_unlock(&args_->mutex),
		"unlock mutex failed\n", 5);

		/* consume message */
		PrintInt(local_rand);
	}

	return NULL;
}


/*******************************************************************************
								   Exercise 5
*******************************************************************************/
/* create argument struct and FSQ */

/* create producers and consumers threads */

/* prodocer */
	/*
	* produce
	* write product to FSQ (Enqueue)
	*/

/* consumer */
	/*
	* read from FSQ (Dequeue)
	* consume
	*/

/* join and destroy*/


/*******************************************************************************
								   Exercise 4
*******************************************************************************/
static void Exercise4(void)
{
	size_t i = 0;
	pthread_t threads_id[THREAD_NUM] = {0};

	/*create argument struct and initialize mutex */
	exe4_args_t args = {NULL, PTHREAD_MUTEX_INITIALIZER, {{0}}, {{0}}};

	/* create semaphores */
	ExitIfError(0 == sem_init(&args.in_semaph, 0, QUEUE_SIZE),
	"init semaphore failed\n", 8);
	ExitIfError(0 == sem_init(&args.out_semaph, 0, 0),
	"init semaphore failed\n", 8);
	
	/* create queue */
	args.queue = FSQcreate(QUEUE_SIZE);

	/* create prudocers & consumers threads */
	for (i = 0; i < THREAD_NUM; i += 2)
	{
		ExitIfError(0 == pthread_create(&threads_id[i], NULL, 
			   (void *(*)(void *))&Producer4,(void *)(&args)),
			   "pthread create producer failed\n", 1);

		ExitIfError(0 == pthread_create(&threads_id[i + 1], NULL, 
				   (void *(*)(void *))&Consumer4, (void *)(&args)), 
				   "pthread create consumer failed\n", 1);
	}

	/* join threads */
	for (i = 0; i < THREAD_NUM; ++i)
	{
		ExitIfError(0 == pthread_join(threads_id[i], NULL), "join failed\n", 2);
	}

	/* destroy list and semaphores */
	FSQdestroy(args.queue);
	ExitIfError(0 == sem_destroy(&args.in_semaph), "sem_destroy failed\n", 13);
	ExitIfError(0 == sem_destroy(&args.out_semaph), "sem_destroy failed\n", 13);
}

static void *Producer4(exe4_args_t *args_)
{
	int local_rand = 0;

	/* create random number */
	local_rand = ProduceRandInt();

	/* lock in-semaphore */
	ExitIfError(0 == sem_wait(&args_->in_semaph), "sem_wait failed\n", 9);

	/* lock mutex */
	ExitIfError(0 == pthread_mutex_lock(&args_->mutex), 
	"lock mutex failed\n", 5);

	/* inqueue random int */
	ExitIfError(-1 != FSQenqueue(args_->queue, &local_rand), 
	"dequeue failed\n", 15);

	/* unlock out-semaphore */
	ExitIfError(0 == sem_post(&args_->out_semaph), "sem_post failed\n", 7);
	
	/* unlock mutex */
	ExitIfError(0 == pthread_mutex_unlock(&args_->mutex),
	"unlock mutex failed\n", 5);

	return NULL;
}


static void *Consumer4(exe4_args_t *args_)
{
	int local_rand = 0;
	
	/* lock out-semaphore */
	ExitIfError(0 == sem_wait(&args_->out_semaph), "sem_wait failed\n", 9);

	/* lock mutex */
	ExitIfError(0 == pthread_mutex_lock(&args_->mutex),
	"lock mutex failed\n", 5);

	/* dequeue random int */
	ExitIfError(-1 != FSQdequeue(args_->queue, &local_rand),
	"enqueue failed\n", 14);

	/* unlock in-semaphore */
	ExitIfError(0 == sem_post(&args_->in_semaph), "sem_post failed\n", 7);

	/* unlock mutex */
	ExitIfError(0 == pthread_mutex_unlock(&args_->mutex),
	"unlock mutex failed\n", 5);

	/* consume */
	PrintInt(local_rand);

	return NULL;
}


/*******************************************************************************
						   Fixed Size Queue Functions
*******************************************************************************/
/* creates Fixed size queue of ints */
static fsq_t *FSQcreate(size_t q_size_)
{
	assert(0 != q_size_);

	return CBufferCreate(q_size_ * sizeof(int));
}

static void FSQdestroy(fsq_t * fsq_)
{
	assert(NULL != fsq_);

	CBufferDestroy(fsq_);
}

/* return -1 if failed */
static ssize_t FSQenqueue(fsq_t *fsq_, const int *src_)
{
	assert(NULL != fsq_);

	return CBufferWrite(fsq_, src_, sizeof(int));
}

/* return -1 if failed */
static ssize_t FSQdequeue(fsq_t *fsq_, int *dest_)
{
	assert(NULL != fsq_);

	return CBufferRead(fsq_, dest_, sizeof(int));
}

/*******************************************************************************
								   Exercise 3
*******************************************************************************/
static void Exercise3(void)
{
	size_t i = 0;
	pthread_t threads_id[THREAD_NUM] = {0};

	/*create argument struct and initialize mutex */
	exe3_args_t args = {NULL, PTHREAD_MUTEX_INITIALIZER, {{0}}};

	/* create semaphore */
	ExitIfError(0 == sem_init(&args.semaphore, 0, 0),
	"init semaphore failed\n", 8);
	
	/* create slist */
	args.list = SListCreate();

	/* create prudocer thread */
	ExitIfError(0 == pthread_create(&threads_id[0], NULL, 
			   (void *(*)(void *))&Producer3,(void *)(&args)),
			   "pthread create producer failed\n", 1);
	
	/* create consumers threads */
	for (i = 1; i < THREAD_NUM; ++i)
	{
		ExitIfError(0 == pthread_create(&threads_id[i], NULL, 
				   (void *(*)(void *))&Consumer3, (void *)(&args)), 
				   "pthread create consumer failed\n", 1);
	}

	/* join threads */
	for (i = 0; i < THREAD_NUM; ++i)
	{
		ExitIfError(0 == pthread_join(threads_id[i], NULL), "join failed\n", 2);
	}

	/* destroy list and semaphore */
	SListDestroy(args.list);
	ExitIfError(0 == sem_destroy(&args.semaphore), "sem_destroy failed\n", 13);
}

static void *Producer3(exe3_args_t *args_)
{	
	size_t local_rand = 0;
	
	while (1)
	{
		/* create random number */
		local_rand = ProduceRandInt();

		/* lock mutex */
		ExitIfError(0 == pthread_mutex_lock(&args_->mutex),
		"lock mutex failed\n", 5);

		/* push random to list */
		SListInsertBefore(args_->list, SListIterEnd(args_->list),
		(void *)local_rand);

		/* unlock (add 1) to semaphore */
		ExitIfError(0 == sem_post(&args_->semaphore), "sem_post failed\n", 7);

		/* unlock mutex */
		ExitIfError(0 == pthread_mutex_unlock(&args_->mutex),
		"unlock mutex failed\n", 5);
	}
	return NULL;
}

static void *Consumer3(exe3_args_t *args_)
{
	size_t local_rand = 0;

	while (1)
	{
		/* lock semaphore (cheak if != 0) and decrements */
		ExitIfError(0 == sem_wait(&args_->semaphore), "sem_wait failed\n", 9);

		/* lock mutex */
		ExitIfError(0 == pthread_mutex_lock(&args_->mutex),
					"lock mutex failed\n", 5);

		/* read from list list */
		local_rand = (size_t)SListGetData(SListIterBegin(args_->list));

		/* pop list */
		SlistRemove(SListIterBegin(args_->list));

		/* unlock mutex */
		ExitIfError(0 == pthread_mutex_unlock(&args_->mutex),
		"unlock mutex failed\n", 5);

		/* consume */
		PrintInt(local_rand);
	}
	
	return NULL;
}

/*******************************************************************************
								   Exercise 2
*******************************************************************************/
static void Exercise2(void)
{
	size_t i = 0;
	pthread_t threads_id[THREAD_NUM] = {0};

	/*create argument struct and initialize mutex */
	exe2_args_t args = {NULL, PTHREAD_MUTEX_INITIALIZER};
	
	/* create slist */
	args.list = SListCreate();

	/* create consumers & prudocers threads */
	for (i = 0; i < THREAD_NUM; i += 2)
	{
		ExitIfError(0 == pthread_create(&threads_id[i], NULL, &Producer2,
		(void *)(&args)), "pthread create producer failed\n", 1);

		ExitIfError(0 == pthread_create(&threads_id[i+1], NULL, &Consumer2,
		(void *)(&args)) ,"pthread create consumer failed\n", 1);
	}

	/* join threads */
	for (i = 0; i < THREAD_NUM; ++i)
	{
		ExitIfError(0 == pthread_join(threads_id[i], NULL), 
		"join producer failed\n", 2);
	}
	
	/* destroy list */
	SListDestroy(args.list);

}

static void *Producer2(void *args_)
{
	/* convert args_ from void to variables */
	pthread_mutex_t mutex  = ((exe2_args_t *)args_)->mutex;
	slist_t *list = ((exe2_args_t *)args_)->list;
	size_t local_rand = 0;

	/* create random number */
	local_rand = ProduceRandInt();

	/* lock mutex */
	ExitIfError(0 == pthread_mutex_lock(&mutex), "lock failed\n", 5);

	/* push random to list */
	SListInsertBefore(list, SListIterEnd(list),(void *)local_rand);

	/* unlock mutex */
	ExitIfError(0 == pthread_mutex_unlock(&mutex), "lock failed\n", 5);
	
	return NULL;
}


static void *Consumer2(void *args_)
{
	/* convert args_ from void to variables */
	pthread_mutex_t mutex  = ((exe2_args_t *)args_)->mutex;
	slist_t *list = ((exe2_args_t *)args_)->list;
	size_t local_rand = 0;
	
	/* lock mutex */
	ExitIfError(0 == pthread_mutex_lock(&mutex), "lock faild\n", 5);

	/* check if list is empty */
	if (0 != SListSize(list))
	{
		/* read from list list */
		local_rand = (size_t)SListGetData(SListIterBegin(list));

		/* pop list */
		SlistRemove(SListIterBegin(list));
	}

	/* unlock mutex */
	ExitIfError(0 == pthread_mutex_unlock(&mutex), "lock faild\n", 5);

	PrintInt(local_rand);

	return NULL;
}

/*******************************************************************************
								   Exercise 1 
*******************************************************************************/
/* TODO - fix to atomic */
static void Exercise1(void)
{
	pthread_t producer = 0, consumer = 0;
	exe1_args_t args = {0};

	/* initilaze args struct */
	args.random_int = 0;
	args.spinlock = 0;

	/* create Producer thread and Consumer thread */
	ExitIfError(0 == pthread_create(&producer, NULL, &Producer1,(void *)(&args))
	,"pthread create producer failed\n", 1);

	ExitIfError(0 == pthread_create(&consumer, NULL, &Consumer1,(void *)(&args))
	,"pthread create consumer failed\n", 1);
	
	/* join threads */
	ExitIfError(0 == pthread_join(producer, NULL), "join producer failed\n", 2);
	ExitIfError(0 == pthread_join(consumer, NULL), "join consumer failed\n", 2);
}

static void *Producer1(void *args_)
{
	/* convert args_ from to (struct *) */
	exe1_args_t *args_struct = args_;
	int local_rand = 0;

	while (1)
	{
		/* assign rand num to local */
		local_rand = ProduceRandInt();
		
		/* time the entrence of producer/consumer */
		while (1 == args_struct->spinlock)
		{
			/* wait */
		}

		/* lock spinlock */
		args_struct->spinlock = 1;

		/* assign local to global */
		args_struct->random_int = local_rand;

		/* unlock spinlock */
		args_struct->spinlock = 0;

		sleep(1);
	}
	
	return NULL;

}

static void *Consumer1(void *args_)
{
	/* convert args_ from to (struct *) */
	exe1_args_t *args_struct = args_;
	int local_rand = 0;

	while (1)
	{
		/* time the entrence of producer/consumer */
		while (1 == args_struct->spinlock)
		{
			/* wait */
		}

		/* lock spinlock */
		args_struct->spinlock = 1;

		/* assign global to local */
		local_rand = args_struct->random_int;

		/* unlock spinlock */
		args_struct->spinlock = 0;

		/* print power of local */
		PrintInt(local_rand);

		sleep(1);
	}

	return NULL;
}

static int ProduceRandInt(void)
{
	int i = 0;
	int rand_num = 0;

	for (i = 0; i < 1000000; ++i)
	{
		rand_num += i;
	}

	rand_num = i + rand() % 1000;

	return rand_num;
}

static void PrintInt(int num_)
{
	printf("random int is: %d\n", num_);
}


