/*******************************************************************************
****** Author: HRD31
****** Date: 21.06.2023
****** Version: 2
*******************************************************************************/
#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h>  /* size_t */
#include "aux_funcs.h" /* is_match_t,  action_func_t */

typedef struct dlist dlist_t;
typedef struct dnode dnode_t;

/* dlist_itr  can change, do not use it */
typedef dnode_t *ditr_t; 


/*******************************************************************************
Description:     	Creates an empty doubly linked list.
Return value:    	Pointer to list in case of success, otherwise NULL.
Time Complexity: 	Determined by system call complexity. 
Note:            	Should call "DListDestroy()" at end of use.
*******************************************************************************/
dlist_t *DListCreate(void);


/*******************************************************************************
Description:     	Deletes a list pointed by "list" from memory.
Time Complexity: 	Determined by system call complexity. 
Notes:           	Undefined behaviour if list is NULL.
*******************************************************************************/
void DListDestroy(dlist_t *list);


/*******************************************************************************
Description:     	Insert 'data' before 'where' in list pointed to by 'list'.
Return value:    	Iterator to inserted data in success otherwise
					DListIterEnd().   
Time Complexity: 	O(1) + system call complexity.
Notes: 			 	Undefined behaviour if list is invalid pointer,
          			Undefined behaviour if 'where' is out of list's range.
*******************************************************************************/
ditr_t DListInsertBefore(dlist_t *list, ditr_t where, void *data);


/*******************************************************************************
Description:     	Deletes the element from the list pointed to by "what".
Return value:    	Iterator to next element.
Time Complexity: 	O(1) + system call complexity.
Notes:           	Undefined behaviour if list is invalid pointer,
					Undefined behaviour if "what" is out of lists range or
                 	if "what" is dummy node (DListEnd()). 
*******************************************************************************/
ditr_t DListRemove(ditr_t what);


/*******************************************************************************
Description:     	Returns number of elements in "list".
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
size_t DListSize(const dlist_t *list);


/*******************************************************************************
Description: 		Checks if "list" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
int DListIsEmpty(const dlist_t *list);


/*******************************************************************************
Description:     	Insert 'data' at the end of the list pointed to by 'list'.
Return value:    	Iterator to inserted data in success, otherwise 
					DListIterEnd().          
Time Complexity: 	O(1) + system call complexity
Notes:           	Undefined behaviour if 'where' is out of list's range,
				 	undefined behaviour if list is invalid pointer.
*******************************************************************************/
ditr_t DListPushBack(dlist_t *list, void *data);


/*******************************************************************************
Description:     	Insert 'data' at the beginning of the list pointed to by 
					'list'.
Return value:    	Iterator to inserted data in success, otherwise 
					DListIterEnd().           
Time Complexity: 	O(1) + system call complexity.
Notes:           	Undefined behaviour if 'where' is out of lists range,
				 	undefined behaviour if list is invalid pointer.
*******************************************************************************/
ditr_t DListPushFront(dlist_t *list, void *data);


/*******************************************************************************
Description:    	Deletes the element from the end of the list pointed to by 
					"list".
Return value:    	The data of the deleted iterator.
Time Complexity: 	O(1) + system call complexity.
Notes: 				Undefined behaviour if list is invalid pointer
		         	Undefined behaviour if list is empty.
*******************************************************************************/
void *DListPopBack(dlist_t *list);


/*******************************************************************************
Description:    	Deletes the element from the beginning of the list pointed 
					out by "list".
Return value:    	The data of the deleted iterator.
Time Complexity: 	O(1) + system call complexity.
Notes: 				Undefined behaviour if list is invalid pointer
		         	Undefined behaviour if list is empty.      
*******************************************************************************/
void *DListPopFront(dlist_t *list);



/*******************************************************************************
Description:  	  	Calls "action_func" on each element between [from, to)
					("to" is exclusive) until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n)
Notes:            	Undefined behavior if "from" or "to" are not valid or do
					not belong to the same list.
*******************************************************************************/
int DListForEach(ditr_t from, ditr_t to, action_func_t action_func,
				 void *user_params);


/*******************************************************************************
Description:  		Insert the sublist that begins with "src_first" and ends
                    with "src_last" before the iterator "dest_where" 
Return value:     	Iterator to the last inserted  element
Time complexity:  	O(1)
Notes:            	Undefined behaviour if "from" or "to" are not valid or do 
                    not belong to the same list.
*******************************************************************************/
ditr_t DListSplice(ditr_t dest_where, ditr_t src_first, ditr_t src_last);



/*******************************************************************************
Description:		Finds the first match according to "match_func" in the range
					[from, to) ( "to" is exclusive).
Return value:   	Iterator to first match or "to" if not found
Time complexity:    O(n)
Note:          		Undefined behaviour if "from" or "to" are not valid or do
					not belong to the same list or if match_func is NULL
*******************************************************************************/
ditr_t DListFind(ditr_t from, ditr_t to, is_match_func_t match_func, 
				void *user_params);


/*******************************************************************************
Description:		Finds all matches according to "match_func" in the range 
					[from, to) ("to" is exclusive).
Return value:   	0 for success, 1 for fail. 
Time complexity:    O(n) + system call complexity
Note:          		Undefined behaviour if "from" or "to" are not valid or do
					not belong to the same list or if match_func is NULL
                    
*******************************************************************************/
int DListMultiFind(ditr_t from, ditr_t to, is_match_func_t match_func, 
				void *user_params, dlist_t *results_list);


/*******************************************************************************
Description:  		Returns next iterator of "itr".
Time complexity:  	O(1)
Note:    			Undefined behaviour if "itr" is Dummy End (DListIterEnd()).
*******************************************************************************/
ditr_t DListIterNext(ditr_t itr);


/*******************************************************************************
Description:  		Returns the iterator before "itr".
Time complexity:  	O(1)
Note:    			Undefined behaviour if "itr" is DListBegin() 
*******************************************************************************/
ditr_t DListIterPrev(ditr_t itr);


/*******************************************************************************
Description: 		Returns an iterator to the first valid elem (invalid element).   
Time complexity:  	O(1)
Note:        		Undefined behaviour if list is invalid pointer
*******************************************************************************/
ditr_t DListIterBegin(const dlist_t *list);


/*******************************************************************************
Description:   		Returns an iterator to the dummy end (invalid element) 
Time complexity:  	O(1)
Note: 				Undefined behaviour if "list" is invalid pointer
*******************************************************************************/
ditr_t DListIterEnd(const dlist_t *list);


/*******************************************************************************
Description: 		Checks if iter1 and iter2 are iterators for the same 
					element.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
*******************************************************************************/
int DListIterIsEqual(ditr_t iter1, ditr_t iter2);


/*******************************************************************************
description:     	Returns pointer to data at 'iter'.
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if 'itr' is out of lists range.
*******************************************************************************/
void *DListGetData(const ditr_t itr);








#endif    /*__DLIST_H__*/

