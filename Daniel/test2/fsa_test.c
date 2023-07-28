/***************************************************************************
Author: Daniel Shabso
Reviewer: Maron
Date of Creation: 6.7.23
Last date of update:   
version: 1
****************************************************************************/
#include <stdio.h>   /*printf                */
#include <stdlib.h>  /* malloc, free, size_t */
#include <assert.h>	 /* assert 				 */
#include "fsa.h"

#define PURPLE "\033[1;35m"
#define CYAN "\033[0;36m"

#define NUM_TESTS 8

/***************************************************************************/
static void TestFsaCountFree(fsa_t *, size_t , int );
static void TestFsaSuggestSize(void);
static void TestFsaAllocAndFree(void);
static void TestFsaAllocAndFreeCases(void);

/***************************************************************************/
int main()
{
    TestFsaSuggestSize();
    TestFsaAllocAndFree();
    TestFsaAllocAndFreeCases();

    return 0;
}

/*******************	HELP FUNCTION	 *********************************/
static void TestFsaCountFree(fsa_t *m_pool, size_t exp_num, int line_num)
{
	assert(NULL != m_pool);
	
	if (FsaCountFree(m_pool) != exp_num)
	{		
		printf("%sFsaCountFree at line %d fail, exp_num - %ld, num - %ld\n",
				PURPLE, line_num, exp_num, FsaCountFree(m_pool));
	}
}

/************************************************************************/
static void TestFsaSuggestSize(void)
{
    size_t i = 0;
    size_t block_size_arr[NUM_TESTS] = {2, 5, 8, 20, 32, 45, 90, 101};
    size_t num_of_block_arr[NUM_TESTS] = {1, 2, 7, 12, 15, 22, 50, 92};
    size_t result_arr[NUM_TESTS] = {16, 24, 64, 296, 488, 1064, 4808, 9576};

    for (i = 0; i < NUM_TESTS; i++)
    {
        if (result_arr[i] != FsaSuggestSize(num_of_block_arr[i], block_size_arr[i]))
        {
            printf("%sFsaSuggestSize failed %d\n", CYAN, __LINE__);
        }
    }
}

/*SIMPLE TEST FOR ALLOC AND FREE*/
static void TestFsaAllocAndFree(void)
{
	fsa_t *pool = NULL;
	size_t size = 0, i = 0, j = 0;
	size_t num_blocks = 10, block_size = 15;
	
	size = FsaSuggestSize(num_blocks, block_size);
	pool = (fsa_t *)malloc(size);
	
	if (NULL == pool)
	{	
		printf("%sMemory wasn't allocated\n", PURPLE);
							
		return;
	}
	
	pool = FsaInit(pool, size, block_size);
		
	for (i = 0; i < 10; i++)
	{
		void *ptrs[10] = {NULL};
		
		for (j = 0; j < i; j++)
		{
			ptrs[j] = FsaAlloc(pool);
			
			TestFsaCountFree(pool, num_blocks - j - 1, __LINE__);
		}

		for (j = 0; j < i; j++)
		{
			FsaFree(pool, ptrs[j]);
			
			TestFsaCountFree(pool, num_blocks - i + j + 1, __LINE__);
		}	
	}
	
	TestFsaCountFree(pool, num_blocks, __LINE__);
	
	free(pool);
	pool = NULL;
}

/*CHECK CASES TEST FOR ALLOC AND FREE*/
static void TestFsaAllocAndFreeCases(void)
{
	fsa_t *pool = NULL;
	
	void *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL, *ptr5 = NULL;
	size_t size = 0, num_blocks = 5, block_size = 7;
	
	size = FsaSuggestSize(num_blocks, block_size);
	pool = (fsa_t *)malloc(size);
	
	if (NULL == pool)
	{	
		printf("%sMemory wasn't allocated\n", PURPLE);
							
		return;
	}
	
	pool = FsaInit(pool, size, block_size);

	ptr1 = FsaAlloc(pool); 
	ptr2 = FsaAlloc(pool); 
	ptr3 = FsaAlloc(pool);
	/*Allocated: 1,2,3*/
		
	if (FsaCountFree(pool) != (num_blocks - 3))
	{
		printf("%sFsaCountFree failed %d\n", CYAN, __LINE__);
	}	
	
	FsaFree(pool, ptr1);
	FsaFree(pool, ptr3);
	/*Allocated: 2*/
	
	if (FsaCountFree(pool) != (num_blocks - 1))
	{
		printf("%sFsaCountFree failed %d\n", CYAN, __LINE__);
	}

	ptr1 = FsaAlloc(pool); 
	ptr3 = FsaAlloc(pool);
	ptr4 = FsaAlloc(pool);
	/*Allocated: 1,2,3,4*/		
	
	if (FsaCountFree(pool) != (num_blocks - 4))
	{
		printf("%sFsaCountFree failed %d\n", CYAN, __LINE__);
	}
		
	FsaFree(pool, ptr2);
	FsaFree(pool, ptr4);
	FsaFree(pool, ptr1);
	/*Allocated: 3*/		
	
	if (FsaCountFree(pool) != (num_blocks - 1))
	{
		printf("%sFsaCountFree failed %d\n", CYAN, __LINE__);
	}
		
	ptr1 = FsaAlloc(pool); 
	ptr2 = FsaAlloc(pool); 
	ptr4 = FsaAlloc(pool);
	ptr5 = FsaAlloc(pool);
	/*Allocated: 1,2,3,4,5*/		
		
	FsaFree(pool, ptr1);
	FsaFree(pool, ptr3);
	FsaFree(pool, ptr5);
	/*Allocated: 2,4*/		
		
	ptr1 = FsaAlloc(pool); 
	ptr5 = FsaAlloc(pool);
	/*Allocated: 1,2,4,5*/		
		
	FsaFree(pool, ptr5);
	FsaFree(pool, ptr1);
	FsaFree(pool, ptr4);

	/*Allocated: 2*/		
		
	free(pool);
	pool = NULL;
}








