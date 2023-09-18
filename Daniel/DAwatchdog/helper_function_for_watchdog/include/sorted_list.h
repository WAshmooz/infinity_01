/*******************************************************************************
****** Author: HRD31
****** Date: 21.06.2023
****** Version: 3
*******************************************************************************/
#ifndef __SORTEDLIST_H__
#define __SORTEDLIST_H__

#include <stddef.h>    /* size_t */

#include "dlist.h"     /* ditr_t */ 
#include "aux_funcs.h" /* ditr_t */ 

typedef struct sorted_list srtlist_t;

/* dlist_itr  can change, do not use it */
typedef struct sorted_list_iter
{
    ditr_t iterator;

    #ifndef NDEBUG
        srtlist_t *list;
    #endif

}srtitr_t; 

/******************************************************************************
Description:     	Performs sort function for sorting.
Return value:    	Returns 0 if new_data == itr_data, Positive number  if 
                    new_data > itr_data, Negative number if new_data < itr_data
******************************************************************************/
typedef int (*sort_func_t) (const void *new_data, const void *itr_data);


/******************************************************************************
Description:     	Creates an empty sorted list sorted by 'sort_func'.
Return value:    	Pointer to list in case of success, otherwise NULL.
Time Complexity: 	Determined by system call complexity. 
Note:            	Should call "SrtListDestroy()" at end of use.
					User must be aware that created list sorted only by 
					'sort_func'
******************************************************************************/
srtlist_t *SrtListCreate(sort_func_t sort_func);


/******************************************************************************
Description:     	Deletes a list pointed by "list" from memory.
Time Complexity: 	Determined by system call complexity. 
Notes:           	Undefined behaviour if list is NULL.
*******************************************************************************/
void SrtListDestroy(srtlist_t *list);


/*******************************************************************************
Description:     	Inserts 'data' to the correct place in 'list' according to 
					'sort_func'provided by user at 'SrtListCreate' 
Return value:    	Iterator to inserted data on success otherwise
					SrtListIterEnd().   
Time Complexity: 	O(n) + system call complexity.
Notes: 			 	Undefined behaviour if list is invalid pointer.
*******************************************************************************/
srtitr_t SrtListInsert(srtlist_t *list, void *data);


/*******************************************************************************
Description:     	Deletes the element from the list pointed to by "what".
Return value:    	Iterator to next element.
Time Complexity: 	O(1) + system call complexity.
Notes:         		Undefined behaviour if "what" is out of list's 
                    range or SrtListIterEnd() 
*******************************************************************************/
srtitr_t SrtListRemove(srtitr_t what);


/*******************************************************************************
Description:     	Returns number of elements in "list".
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
size_t SrtListSize(const srtlist_t *list);


/*******************************************************************************
Description: 		Checks if "list" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
int SrtListIsEmpty(const srtlist_t *list);


/*******************************************************************************
Description:    	Deletes the element from the end of the list pointed to by 
					"list".
Return value:    	The data of the deleted iterator.
Time Complexity: 	O(1) + system call complexity.
Notes: 				Undefined behaviour if list is invalid pointer
		         	Undefined behaviour if list is empty.
*******************************************************************************/
void *SrtListPopBack(srtlist_t *list);


/*******************************************************************************
Description:    	Deletes the element from the beginning of the list pointed 
					out by "list".
Return value:    	The data of the deleted iterator.
Time Complexity: 	O(1) + system call complexity.
Notes: 				Undefined behaviour if list is invalid pointer
		         	Undefined behaviour if list is empty.      
*******************************************************************************/
void *SrtListPopFront(srtlist_t *list);



/*******************************************************************************
Description:  	  	Calls "action_func" on each element between [from, to)
					("to" is exclusive) until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n)
Notes:            	Undefined behavior if "from" or "to" are not valid or do
					not belong to the same list.
					User responsible that 'action_func' should not affect sort 
					order.  
*******************************************************************************/
int SrtListForEach(srtitr_t from, srtitr_t to, action_func_t action_func,
				 void *user_params);

/*******************************************************************************
Description:		Finds the first match according to "sort_func" 
					(sorting criteria) provided by user at 'SrtListCreate',
                    in the range  [from, to) ("to" is exclusive).
Return value:   	Iterator to first match in list if found, otherwise "to".
Time complexity:    O(n)
Note:          		Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
srtitr_t SrtListFind(const srtlist_t *list, srtitr_t from, srtitr_t to, void *data);


/*******************************************************************************
Description:		Finds first match according to "match_func" in the range 
					[from, to) ("to" is exclusive).
Return value:   	Iterator to first match  if found , otherwise "to" 
Time complexity:    O(n) + system call complexity
Note:          		Undefined behaviour if "from" or "to" are not valid or do
					not belong to the same list or if match_func is NULL
*******************************************************************************/
srtitr_t SrtListFindIf(srtitr_t from, srtitr_t to, is_match_func_t match_func, 
				void *user_params);

/*******************************************************************************
Description:  		Merges dest_list and src_list into dest_list. src_list
                    should remain empty.
Time complexity:  	O(n + m)
Notes:            	Undefined behaviour if lists are sorted according to different 
                    sorting criteria, or pointers are not valid
*******************************************************************************/
void SortListMerge(srtlist_t *dest_list, srtlist_t *src_list);


/*******************************************************************************
Description:  		Returns next iterator of "itr".
Time complexity:  	O(1)
Note:    			Undefined behaviour if "itr" is Dummy End (SrtListIterEnd()).
*******************************************************************************/
srtitr_t SrtListIterNext(srtitr_t itr);


/*******************************************************************************
Description:  		Returns the iterator before "itr".
Time complexity:  	O(1)
Note:    			Undefined behaviour if "itr" is SrtListBegin() 
*******************************************************************************/
srtitr_t SrtListIterPrev(srtitr_t itr);


/*******************************************************************************
Description: 		Returns an iterator to the first valid elem (nvalid element).   
Time complexity:  	O(1)
Note:        		Undefined behaviour if list is invalid pointer
*******************************************************************************/
srtitr_t SrtListIterBegin(const srtlist_t *list);


/*******************************************************************************
Description:   		Returns an iterator to the dummy end (invalid element) 
Time complexity:  	O(1)
Note: 				Undefined behaviour if "list" is invalid pointer
*******************************************************************************/
srtitr_t SrtListIterEnd(const srtlist_t *list);


/*******************************************************************************
Description: 		Checks if iter1 and iter2 are iterators to the same 
					element.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
*******************************************************************************/
int SrtListIterIsEqual(srtitr_t iter1, srtitr_t iter2);


/*******************************************************************************
description:     	Returns pointer to data at 'iter'.
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if 'itr' is out of lists range.
*******************************************************************************/
void *SrtListGetData(const srtitr_t itr);

#endif    /*__SORTEDLIST_H__*/

