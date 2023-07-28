/*******************************************************************************
****** Author: HRD31
****** Date: 16.07.2023
****** Version: 2.2
****** Description: Binary Search Tree DS header.
*******************************************************************************/
#ifndef __BST_H__
#define __BST_H__

#include <stddef.h>    /* size_t        */
#include "aux_funcs.h" /* action_func_t */

typedef struct bst bst_t;
typedef struct tree_node tree_node_t;

/* tree_node_t  can change, do not use it */
typedef struct
{
    tree_node_t *node;  
} bst_itr_t;

/******************************************************************************
Description:     	Creates a binary search tree sorted by 'is_before_func'.
Return value:    	Pointer to tree in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call "BstDestroy()" at end of use
					User must be aware that created tree sored only by 
					'is_before_func'
******************************************************************************/
bst_t *BstCreate(is_before_t is_before_func);


/******************************************************************************
Description:     	Deletes a tree pointed by "tree" from memory.
Time Complexity: 	O(n). 
Notes:           	Undefined behaviour if tree is NULL.
*******************************************************************************/
void BstDestroy(bst_t *tree);


/*******************************************************************************
Description:     	Inserts 'data' to the correct place in 'tree' according to 
					'sort_func' provided by user at 'BstCreate' 
Return value:    	Iterator to inserted data on success otherwise NULL/
					BstIterEnd().   
Time Complexity: 	O(log(n)), (worst case O(n)).
Notes: 			 	Undefined behaviour if tree is invalid pointer.
*******************************************************************************/
bst_itr_t BstInsert(bst_t *tree, void *data);


/*******************************************************************************
Description:     	Deletes the element from the tree pointed to by "what".
Return value:    	Iterator to next element.
Time Complexity: 	O(log(n))
Notes:         		Undefined behaviour if "what" is out of tree's 
                    range or BstIterEnd() 
*******************************************************************************/
bst_itr_t BstRemove(bst_itr_t what);


/*******************************************************************************
Description:     	Returns number of elements in "tree".
Time Complexity: 	O(n).
Notes:			 	Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
size_t BstSize(const bst_t *tree);


/*******************************************************************************
Description: 		Checks if "tree" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
int BstIsEmpty(const bst_t *tree);

/*******************************************************************************
Description:  	  	Calls "action_func" on each element between [from, to)
					("to" is exclusive) until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n log n)
Notes:            	Undefined behavior if "from" or "to" are not valid or do
					not belong to the same tree.
					User responsible that 'action_func' should not affect sort 
					order.  
*******************************************************************************/
int BstForEach(bst_itr_t from, bst_itr_t to, action_func_t action_func,
				 void *user_params);


/*******************************************************************************
Description:		Finds the first match according to "is_before_func" 
Return value:   	Iterator to first match in tree if found, otherwise
                    BstIterEnd().
Time complexity:    O(log n)
Note:          		Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
bst_itr_t BstFind(const bst_t *tree, const void *data);


/*******************************************************************************
Description:  		Returns next iterator of "itr".
Time complexity:  	O(log(n)) ,(worst case O(n)).
Note:    			Undefined behaviour if "itr" is BstIterEnd().
*******************************************************************************/
bst_itr_t BstIterNext(bst_itr_t itr);


/*******************************************************************************
Description:  		Returns the iterator before "itr".
Time complexity:  	O(log(n)) ,(worst case O(n)).
Note:    			Undefined behaviour if "itr" BstIterBegin() 
*******************************************************************************/
bst_itr_t BstIterPrev(bst_itr_t itr);


/*******************************************************************************
Description: 		Returns an iterator to the first valid element.   
Time complexity:  	O(log(n)), (worst case O(n)).
Note:        		Undefined behaviour if "tree" is invalid pointer
*******************************************************************************/
bst_itr_t BstIterBegin(const bst_t *tree);


/*******************************************************************************
Description:   		Returns an iterator to the dummy end (invalid element) 
Time complexity:  	O(1).
Note: 				Undefined behaviour if "tree" is invalid pointer
*******************************************************************************/
bst_itr_t BstIterEnd(const bst_t *tree);


/*******************************************************************************
Description: 		Checks if iter1 and iter2 are iterators to the same 
					element.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
*******************************************************************************/
int BstIterIsEqual(bst_itr_t itr1, bst_itr_t itr2);


/*******************************************************************************
description:     	Returns pointer to data at 'itr'.
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if 'itr' is out of tree range.
*******************************************************************************/
void *BstGetData(const bst_itr_t itr);



#endif    /*__BST_H__*/
