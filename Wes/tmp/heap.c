/*******************************************************************************
****** Author: HRD31
****** Date: 31.07.2023
****** Version: 1
****** Description: Data structure that maps keys to values.
*******************************************************************************/
#include <stdlib.h>/* size_t, malloc, free, srand, rand	*/
#include "vector.h"
#include "heap.h"
#include <assert.h>

#include <stdio.h>		/* printf */

#define CAPACITY 30

enum { FALSE = 0, TRUE = 1 };
enum { SUCCESS = 0, FAIL = 1 };

struct heap
{
    vector_t *vector;
    is_before_t is_before_func;
};

/*******************************************************************************
							Helper Functions 	
*******************************************************************************/
void SwapPtr(size_t *ptr1, size_t *ptr2) 
{
    size_t temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

void SwapArrElements(void **arr, size_t idx1, size_t idx2) 
{
    void *temp = arr[idx1];
    arr[idx1] = arr[idx2];
    arr[idx2] = temp;
}

size_t GetParentIdx(size_t idx_child)
{
	if (0 == idx_child || 1 == idx_child || 2 == idx_child)
	{
		return 0;
	}
	
	else if (0 == (idx_child % 2))
	{
		return ((idx_child - 2) / 2);
	}
	else 
	{
		return ((idx_child - 1) / 2);
	}
} /* option 	(i + 1) /2 - (i != 0) */

void PrintHeap(heap_t *heap)
{
	vector_t *vec = heap->vector;
	int i = 0, j = 0, level = 0;
	int curIdx = 0;
	int nodes_on_level = 1;
	int curNode;
	int safeStop = 100;
	int treeSpaces1[] = { 
		126, 0,
		62,  126,
		30, 62,
		14, 30,
		6, 14,
		2,  6,
		0,  2 };
	int treeSpaces2[] = { 
		62,  0,
		30, 62,
		14, 30,
		6, 14,
		2,  6,
		0,  2 };
	
	int *treeSpaces = treeSpaces2;
	int spacesBefore, spacesInBetween;
	size_t vectorSize = VectorSize(vec);
	
	/* for (nodes_on_level = 1, i = 0; i < vectorSize; ) */
	while (--safeStop && curIdx < *(int *)vectorSize) 
	{
		spacesBefore = treeSpaces[level++];
		spacesInBetween = treeSpaces[level++]; 
		printf("%*s", spacesBefore, "");
		
		for (i = 0; i < nodes_on_level; ++i, ++curIdx)
		{
			curNode = *(int*)VectorGetAccess(vec, curIdx);
			printf("%02d", curNode);
			if (i % 2) {
				printf("%*s", spacesInBetween, "");
			}
			else {
				for (j = 0; j < spacesInBetween; ++j) printf("-");
				/* printf("%-s", half-4, ""); */
			}
		}
		nodes_on_level *= 2;
		printf("\n");
	}
	printf("\n\n--------------------------------------------------------------\n");
}

/*******************************************************************************
Description:     	Creates an empty heap, according to 'is_before_func'.
Return value:    	Pointer to heap in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call 'HeapDestroy()' at end ofuse.
*******************************************************************************/
heap_t *HeapCreate(is_before_t is_before_func)
{
	heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
	
	if(NULL == heap)
	{
		return NULL;
	}
	
	heap->vector = VectorCreate(CAPACITY, sizeof(size_t));
	heap->is_before_func = is_before_func;
	
	return heap;
}
/*******************************************************************************
Description:     	Deletes a heap pointed by 'heap' from memory.
Time Complexity: 	O(1) 
Notes:           	Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
void HeapDestroy(heap_t *heap)
{
	assert(NULL != heap);
	
	VectorDestroy(heap->vector);
	free(heap); heap = NULL;
}
/*******************************************************************************
Description:     	Add 'data' to heap pointed by 'heap'.
Return value:    	0 for success, 1 for fail.         
Time Complexity: 	Amortized O(log(n)).
Notes:              Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
int HeapPush(heap_t *heap, void *data)
{
	size_t idx_child = HeapSize(heap);
	size_t idx_parent = GetParentIdx(idx_child);
	
	size_t *child = VectorGetAccess(heap->vector, idx_child);		
	size_t *parent = VectorGetAccess(heap->vector, idx_parent);

	assert(NULL != heap);	
	assert(NULL != data);
	
	if (VectorPushBack(heap->vector, data))
	{
		return FAIL;
	}
	
	while( TRUE == heap->is_before_func(*(void**)child, *(void**)parent) )
	{
			SwapPtr(child, parent);

			idx_child = idx_parent;
			idx_parent = GetParentIdx(idx_child);
			child = VectorGetAccess(heap->vector, idx_child);
			if(child == VectorGetAccess(heap->vector, 0))
			{
				break;
			}
			parent = VectorGetAccess(heap->vector, idx_parent);
	}	
	
	return SUCCESS;
}

/*******************************************************************************
Description:       Deletes the first element in 'heap'.
Time Complexity:   O(log(n))
Notes:             Undefined behaviour if 'heap' is invalid pointer.          
*******************************************************************************/
void HeapPop(heap_t *heap);


/*******************************************************************************
Description:   		Returns data of the first element in 'heap'. 
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
void *HeapPeek(const heap_t *heap)
{
	size_t idx = (HeapSize(heap) - 1);		
	
	assert(NULL != heap);
	
	return *(void**)VectorGetAccess(heap->vector, idx);
}

/*******************************************************************************
Description: 		Checks if 'heap' is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
int HeapIsEmpty(const heap_t *heap)
{
	assert(NULL != heap);

	return (0 == VectorSize(heap->vector)? TRUE : FALSE);	
}
/*******************************************************************************
Description:     	Returns number of elements in 'heap'
Time Complexity: 	O(1)
Notes:			 	Undefined behaviour if 'heap' is invalid pointer
*******************************************************************************/
size_t HeapSize(const heap_t *heap)
{
	assert(NULL != heap);
	
	return VectorSize(heap->vector);
}

/*******************************************************************************
Description:  	  	Erase the element with 'data' found by 'is_before_func' from
                    a heap pointed by 'heap'.
Return value 		0 for success otherwise 1.
Time complexity:  	O(n)
Notes:            	Undefined behaviour if 'heap' is invalid pointer.
*******************************************************************************/
int HeapRemove(heap_t *heap, is_match_func_t is_match, void *param);




