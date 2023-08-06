/******************************************************************************  
****	Author: 	Daniel Shabso
****	Reviewer: 	 
****	Date: 		02.08.23
****	Version: 	0
****    Description: Data structure - Heap.
******************************************************************************/
#include <stdlib.h>/* size_t, malloc, free, srand, rand	*/
#include "vector.h"
#include "heap.h"
#include <assert.h>

#include <stdio.h>		/* printf */

#define CAPACITY 30

enum { FALSE = 0, TRUE = 1 };
enum { SUCCESS = 0, FAIL = 1 };
enum { LEFT = 0, RIGHT = 1 };

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

/****************************************/
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

/****************************************/
/*left_or_right : 0 = LEFT , 1 = RIGHT*/
size_t GetChildIdx(size_t idx_parent, size_t left_or_right)
{
	size_t factor = (1 + left_or_right);

	return ((idx_parent / 2) - factor);
} /* option 	(i / 2) / - 1 -(i != 0) */

/****************************************/
size_t GoToMaxChild(heap_t *heap, size_t idx_parent)
{
 	assert(NULL != heap);
 	{
		size_t max_child = (heap->is_before_func(GetChildIdx(idx_parent, LEFT),
												GetChildIdx(idx_parent, RIGHT)))
	 	return max_child;
	|
}

/****************************************/
static void HeapifyUp(heap_t *heap, size_t idx_child)
{
	size_t idx_parent = GetParentIdx(idx_child);
		
	void *child = VectorGetAccess(heap->vector, idx_child);		
	void *parent = VectorGetAccess(heap->vector, idx_parent);

 	assert(NULL != heap);
 	
 	while( TRUE == heap->is_before_func(child, parent) )
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
}	

/****************************************/
static void HeapifyDown(heap_t *heap, size_t idx_to_remove)
{
    assert(NULL != heap);

    size_t size = HeapSize(heap);
    size_t idx_parent = idx_to_remove;
    size_t max_child_idx = 0;

    while (idx_parent < size)
    {
        max_child_idx = GoToMaxChild(heap, idx_parent);

        if (max_child_idx >= size)
        {
            break;
        }

        void *parent = VectorGetAccess(heap->vector, idx_parent);
        void *max_child = VectorGetAccess(heap->vector, max_child_idx);

        if (heap->is_before_func(parent, max_child))
        {
            SwapPtr(parent, max_child);
            idx_parent = max_child_idx;
        }
        else
        {
            break;
        }
    }
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

	assert(NULL != heap);	
	assert(NULL != data);
		
	if (VectorPushBack(heap->vector, &data))
	{
		return FAIL;
	}	

	HeapifyUp(heap, idx_child);
		
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
	
	return VectorGetAccess(heap->vector, idx);
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
int HeapRemove(heap_t *heap, is_match_func_t is_match, void *param)
{
    assert(NULL != heap);
    assert(NULL != is_match);
	{
		size_t size = HeapSize(heap);
		size_t idx_to_remove = 0;

		for (size_t i = 0; i < size; i++)
		{
			void *element = VectorGetAccess(heap->vector, i);
		    if (is_match(element, param))
		    {
		    	idx_to_remove = i;
		    	break;
		    }
		}

		if (idx_to_remove >= size)
		{
			return FAIL; /* Element not found */
		}

		/* Replace the element to remove with the last element in the heap */
		void *last_element = VectorGetAccess(heap->vector, size - 1);
		VectorSetElement(heap->vector, idx_to_remove, last_element);

		/* Remove the last element from the heap (to maintain the heap size) */
		VectorPopBack(heap->vector);

		/* Perform HeapifyDown to maintain the heap property */
		HeapifyDown(heap, idx_to_remove);

		return SUCCESS;
	}
}

/*******************************************************************************
				Print function for testing
*******************************************************************************/
void PrintHeap(heap_t *heap, print_element_t printFunc)
{
	vector_t *vec = heap->vector;
	int i = 0, j = 0, level = 0;
	int curIdx = 0;
	int nodes_on_level = 1;
	void *curNode;
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
	
	while (--safeStop && curIdx < vectorSize)
	{
		spacesBefore = treeSpaces[level++];
		spacesInBetween = treeSpaces[level++]; 
		printf("%*s", spacesBefore, "");
		
		for (i = 0; i < nodes_on_level && curIdx < vectorSize; ++i, ++curIdx)
		{
			curNode = VectorGetAccess(vec, curIdx);
			printFunc(curNode);
			
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
	printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}


