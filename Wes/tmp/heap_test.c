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

void PrintHeap(heap_t *heap, PrintElement printFunc);
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

static int IsBefore1(const void *lhs, const void *rhs)
{
	/* printf("IsBefore: lhs = %d,  rhs = %d\n", *(int *)lhs, *(int *)rhs); */
	return *(int *)lhs < *(int *)rhs;
}


typedef struct MyData
{
	char ch;
	size_t num1;
	size_t num2;
} MyData_t;

static int IsBefore(const void *lhs, const void *rhs)
{
	const MyData_t *l = (const MyData_t*)lhs;
	const MyData_t *r = (const MyData_t*)rhs;
	return l->ch < r->ch;
}

void PrintFunc(const void *element)
{
	const MyData_t *elm = *(const MyData_t **)element;
	
	printf("%2c", elm->ch);
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
	/* int arr[NUM_OF_TESTS] = {11, 10, 3, 4, 5, 6, 7, 8, 9, 2}; */
	MyData_t arr[NUM_OF_TESTS] = {
		{'a', 11, 33}, {'b', 10, 33}, {'c', 3, 33}, {'d', 4, 33}, 
		{'e', 5, 33}, {'f', 6, 33}, {'g', 7, 33}, {'h', 8, 33}, {'i', 9, 33}, {'j', 2, 33}
	};
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
		PrintHeap(heap_ptr, PrintFunc);
	}
	arr[0].ch = 'X';
	/* arr[0] = 888; */
	PrintHeap(heap_ptr, PrintFunc);
	
	HeapDestroy(heap_ptr);
	heap_ptr = NULL;
}

