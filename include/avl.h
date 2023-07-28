/*******************************************************************************
****** Author: HRD31
****** Date: 24.07.2023
****** Version: 2
****** Description: Binary Search Tree (recursive) DS header.
*******************************************************************************/
#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h>    /* size_t        */
#include "aux_funcs.h" /* action_func_t, is_before_t */

typedef struct avl avl_t;

/******************************************************************************
Description:     	Creates an AVL tree sorted by 'is_before_func'.
Return value:    	Pointer to tree in case of success, otherwise NULL.
Time Complexity: 	O(1).
Note:            	Should call "AvlDestroy()" at end of use
					User must be aware that created tree sored only by 
					'is_before_func'.
******************************************************************************/
avl_t *AvlCreate(is_before_t is_before_func);


/******************************************************************************
Description:     	Deletes a tree pointed to by "tree" from memory.
Time Complexity: 	O(n). 
Notes:           	Undefined behaviour if tree is NULL.
*******************************************************************************/
void AvlDestroy(avl_t *tree);


/*******************************************************************************
Description:     	Deletes the element from the tree pointed to by "what".
Time Complexity: 	O(log(n)).
Notes               Undefined behaviour if no match found.
*******************************************************************************/
void AvlRemove(avl_t *tree, const void *data);


/*******************************************************************************
Description:     	Inserts 'data' to the correct place in 'tree' according to 
					'is_before_func' provided by user at 'AvlCreate' 
Return value:    	0 incase of success otherwise 1.  
Time Complexity: 	O(log(n)), (worst case O(n)).
Notes: 			 	Undefined behaviour if tree is invalid pointer.
*******************************************************************************/
int AvlInsert(avl_t *tree, void *data);


/*******************************************************************************
Description:     	Returns 'tree' height.
Time Complexity: 	O(log(n)).
Notes:			 	Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
size_t AvlHeight(const avl_t *tree);


/*******************************************************************************
Description:     	Returns number of elements in "tree".
Time Complexity: 	O(n).
Notes:			 	Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
size_t AvlSize(const avl_t *tree);


/*******************************************************************************
Description: 		Checks if "tree" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
int AvlIsEmpty(const avl_t *tree);

/*******************************************************************************
Description:  	  	Calls "action_func" on all elements in tree until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n log n)
Notes:            	User responsible that 'action_func' should not affect sort 
					order.
*******************************************************************************/
int AvlForEachPreorder(avl_t *tree, action_func_t action_func, void *user_params);


/*******************************************************************************
Description:  	  	Calls "action_func" on all elements in tree until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n log n)
Notes:            	User responsible that 'action_func' should not affect sort 
					order.
*******************************************************************************/
int AvlForEachInorder(avl_t *tree, action_func_t action_func, void *user_params);


/*******************************************************************************
Description:  	  	Calls "action_func" on all elements in tree until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n log n)
Notes:            	User responsible that 'action_func' should not affect sort 
					order.
*******************************************************************************/
int AvlForEachPostorder(avl_t *tree, action_func_t action_func, void *user_params);


/*******************************************************************************
Description:		Finds the first match according to "is_before_func" 
Return value:       Pointer to data incase of success, otherwise NULL.
Time complexity:    O(log n)
Note:          		Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
void *AvlFind(const avl_t *tree, const void *data);

#endif    /*__AVL_H__*/





