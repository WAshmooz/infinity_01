/******************************************************************************  
****	dlist_test.c
****	Author: 	Daniel Shabso
****	Reviewer: 	 
****	Date: 		02.08.23
****	Version: 	0
****    Description: Data structure - Heap.
******************************************************************************/
#include <stdio.h>		/* printf 								*/
#include <stdlib.h> 	/* size_t, malloc, free, srand, rand	*/
#include <string.h> 	/* strncmp								*/

#include "heap.h"
#include "vector.h"

enum { FALSE = 0, TRUE = 1 };
enum { SUCCESS = 0, FAIL = 1 };

#define NUM_OF_TESTS 10

/*******************************************************************************
							function declaration
*******************************************************************************/
static void TestCreateAndDestroy(void);
static void TestInsert(void);

static int IsBefore(const void *lhs, const void *rhs);
/*******************************************************************************
									main
*******************************************************************************/
int main(void)
{
	TestCreateAndDestroy();
	TestInsert();
	
	return 0;
}

/*******************************************************************************
								Helper Functions
*******************************************************************************/

static int IsBefore(const void *lhs, const void *rhs)
{
	return *(int *)lhs < *(int *)rhs;
}

/*******************************************************************************
									TESTS
*******************************************************************************/
/*********	Test 1	***********/
static void TestCreateAndDestroy(void)
{
	int i = 0;
	heap_t *heap_ptr = NULL;
	
	for (i = 0; i < NUM_OF_TESTS; i++)
	{
		heap_ptr = HeapCreate(IsBefore);
		
		if ( 0 != HeapSize(heap_ptr) ) 
		{
			printf("TestForCreateAndDestroy failed DListSize != 0 for i = %d\n", i);	
		}
		
		if ( 1 != HeapIsEmpty(heap_ptr) )
		{
			printf("TestForCreateAndDestroy failed DListIsEmpty != 1 for i = %d\n", i);	
		}
	
		HeapDestroy(heap_ptr);
	}
	heap_ptr = NULL;
}

/*********	Test 2	***********/
static void TestInsert(void)
{
	size_t i = 0;
	heap_t *heap_ptr = NULL;
	int arr[NUM_OF_TESTS] = {11, 10, 3, 4, 5, 6, 7, 8, 9, 2};
	int *data = NULL;
		
	heap_ptr = HeapCreate(IsBefore);
			
	for (i = 0; i < NUM_OF_TESTS; i++)
	{
		if (HeapPush(heap_ptr, &arr) != SUCCESS)
		{
			printf("HeapPush failed for i = %ld\n", i);
		}
		
		data = HeapPeek(heap_ptr);
		printf("HeapPeek = %d\n", *data);
		printf("HeapPeek = %ld\n", HeapSize(heap_ptr));
						
		if ( (i+1) != HeapSize(heap_ptr) ) 
		{
			printf("HeapPush failed HeapSize for i = %ld\n", i);	
		}
				printf("-------------------------------\n");
	}			
	  			
	HeapDestroy(heap_ptr);	
	heap_ptr = NULL;
}



