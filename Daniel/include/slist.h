/*******************************************************************************
****** Author: HRD31
****** Date: 07.06.2023
****** Version: 3
*******************************************************************************/
#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>  /* size_t */

typedef struct slist slist_t;
typedef struct snode snode_t;

/* slist_iter  can change, do not use it */
typedef snode_t *sitr_t; 

/*********************************************************************************
description:     	performs specific action on data.
return value:    	returns 0 for success, 1 for fail
*********************************************************************************/
typedef int(*action_func_t)(void *data, void *user_params);

/*********************************************************************************
description:     	checks if data applies to specific criteria.
return value:    	returns 1 for true, 0 for false
*********************************************************************************/
typedef int (*is_match_func_t)(void *data, void *user_params);

/*******************************************************************************
description:     	create an empty singly linked list
return value:    	pointer to list in case of success, otherwise NULL.
Time Complexity: 	determined by system call complexity. 
Note:            	should call "SListDestroy()" at end of use.
*******************************************************************************/
slist_t *SListCreate(void);


/*******************************************************************************
description:     	deletes a list pointed by "list" from memory.
Time Complexity: 	determined by system call complexity. 
Notes:           	undefined behaviour if list is NULL.
*******************************************************************************/
void SListDestroy(slist_t *list);


/*******************************************************************************
description:     	insert 'data' before 'where' in list pointed by 'list'.
return value:    	iterator to inserted data in success,
                 	otherwise SListIterEnd();            
Time Complexity: 	O(1) + system call complexity
Notes:           	undefined behaviour if 'where' is out of lists range
				 	undefined behaviour of list is invalid pointer.
*******************************************************************************/
sitr_t SListInsertBefore(slist_t *list, sitr_t where, void *data);


/*******************************************************************************
description:     deletes the element in list pointed by "what".
return value:    iterator to next element
Time Complexity: O(1)
Notes:           undefined behaviour if "what" is out of lists range or
                 if "what" is dummy node (SListEnd())          
*******************************************************************************/
sitr_t SlistRemove(sitr_t what);


/*******************************************************************************
description:     	returns number of elements in "list"
Time Complexity: 	O(n)
Notes:			 	undefined behaviour of list is invalid pointer
*******************************************************************************/
size_t SListSize(const slist_t *list);


/*******************************************************************************
description:     	return data at 'iter'  
Time Complexity: 	O(1)
Notes:           	undefined behaviour if 'itr' is out of lists range.
*******************************************************************************/
void *SListGetData(sitr_t iter);


/*******************************************************************************
description:     	sets 'itr' to 'new_data'.
Time Complexity: 	O(1)
Notes:           	undefined behaviour if 'itr' is out of lists range.
*******************************************************************************/
void SListSetData(sitr_t itr, void *new_data);

/*******************************************************************************
description:  		returns next iterator of "itr"  
Time complexity:  	O(1)
Note:    			undefined behaviour if "itr" is Dummy End ( SListIterEnd()).
*******************************************************************************/
sitr_t SListIterNext(sitr_t itr);

/*******************************************************************************
description: 		returns iterstor to first valid element in "list"   
Time complexity:  	O(1)
Note:        		Dummy End (SListIterEnd()) in case of empty list.
             		undefined behaviour of list is invalid pointer
*******************************************************************************/
sitr_t SListIterBegin(const slist_t *list);


/*******************************************************************************
description:   		returns iterator to dummy end ( invalid element) 
Time complexity:  	O(1)
Note: 				undefined behaviour of "list" is invalid pointer
*******************************************************************************/
sitr_t SListIterEnd(const slist_t *list);


/*******************************************************************************
description: 		checks if iter1 and iter2 are iterators for the same element
return value:   	1 for true, 0 for false
Time complexity:  	O(1)
*******************************************************************************/
int SListIterIsEqual(sitr_t iter1, sitr_t iter2);


/*******************************************************************************
description:  	  	calls "action_func" on each element between [from, to) 
                  	( "to" is exclusive) untill fail
return value:     	0 for success, 1 for fail
Time complexity:  	O(n)
Notes:            	undefined behaviour if "from" or "to" are not valid or not 
				  	belongs to the same list.
*******************************************************************************/
int SListForEach(sitr_t from, sitr_t to, action_func_t action_func,
				 void *user_params)	;

/*******************************************************************************
description:   returns iterator to the first match according to "match_func" in range 
               [from, to) ( "to" is exclusive).
 
return value:   	 iterator to first match or "to" if not found
Time complexity:    O(n)
Note:          	undefined behaviour if "from" or "to" are not valid or not 
				belongs to the same list or if match_func is NULL
*******************************************************************************/
sitr_t SListFind(sitr_t from, sitr_t to, 
						is_match_func_t match_func, void *user_params);


/*******************************************************************************
Description:  	  	Appends "src" head to "dest" tail. 
                    src remains empty and valid.
Time complexity:  	O(1)
Notes:            	Undefined behaviour if queues are invalid pointers.
*******************************************************************************/
slist_t *SListAppend(slist_t *dest, slist_t *src);


#endif    /*__SLIST_H__*/
