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

#include "../include/heap.h"
#include "../include/vector.h"

enum { FALSE = 0, TRUE = 1 };
enum { SUCCESS = 0, FAIL = 1 };

#define NUM_OF_TESTS 10

/*******************************************************************************
							function declaration
*******************************************************************************/
static void TestCreateAndDestroy(void);
static void TestInsert(void);
static void TestInsert2(void);

void PrintHeap(heap_t *heap);
static int IsBefore(const void *lhs, const void *rhs);
/*******************************************************************************
									main
*******************************************************************************/
int main(void)
{
	TestCreateAndDestroy();
	/*TestInsert();*/
	TestInsert2();	
	return 0;
}

/*******************************************************************************
								Helper Functions
*******************************************************************************/

static int IsBefore(const void *lhs, const void *rhs)
{
	/* printf("IsBefore: lhs = %d,  rhs = %d\n", *(int *)lhs, *(int *)rhs); */
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
		if (HeapPush(heap_ptr, &arr[i]) != SUCCESS)
		{
			printf("HeapPush failed for i = %ld\n", i);
		}
		
		data = HeapPeek(heap_ptr);
		printf("HeapPeek = %d\n", *data);
		printf("HeapSize = %ld\n", HeapSize(heap_ptr));
						
		if ( (i+1) != HeapSize(heap_ptr) ) 
		{
			printf("HeapPush failed HeapSize for i = %ld\n", i);	
		}
				printf("-------------------------------\n\n\n");
	}			
	  			
	HeapDestroy(heap_ptr);	
	heap_ptr = NULL;
}


static void TestInsert2(void)
{
	size_t i = 0;
	heap_t *heap_ptr = NULL;
	int arr[NUM_OF_TESTS] = {11, 10, 3, 4, 5, 6, 7, 8, 9, 2};
	int *data = NULL;
	
	heap_ptr = HeapCreate(IsBefore);
	
	for (i = 0; i < NUM_OF_TESTS; i++)
	{
		if (HeapPush(heap_ptr, &arr[i]) != SUCCESS)
		{
			printf("HeapPush failed for i = %ld\n", i);
		}
		
		data = HeapPeek(heap_ptr);
		printf("HeapPeek = %d, ", *data);
		printf("HeapSize = %ld\n", HeapSize(heap_ptr));
		
		if ( (i+1) != HeapSize(heap_ptr) ) 
		{
			printf("HeapPush failed HeapSize for i = %ld\n", i);	
		}
		PrintHeap(heap_ptr);
	}
	
	PrintHeap(heap_ptr);
	
	HeapDestroy(heap_ptr);
	heap_ptr = NULL;
}

