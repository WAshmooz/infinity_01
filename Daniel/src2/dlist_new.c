/*******************************************************************************
****** Author: 		Daniel
****** Reviewer:    Shai	
****** Date: 		19.06.2023
****** Version:		1
*******************************************************************************/

/**********************		Headers		***************************************/

#include <stdio.h>  /* size_t  */
#include <stdlib.h> /* malloc, free */ 
#include <assert.h> /* assert  */ 
#include "dlist.h"

#define ITR_TO_NODE(itr) ( (dnode_t *)itr )
#define NODE_TO_ITR(node)  ( (ditr_t)node )
/******************************************************************************/
struct dnode 
{
	void *data;
	dnode_t *next;
	dnode_t *prev;
};

struct dlist
{
	dnode_t head;
    dnode_t tail;
};

/*******************************************************************************
Description:     	Creates an empty doubly linked list.
Return value:    	Pointer to list in case of success, otherwise NULL.
Time Complexity: 	Determined by system call complexity. 
Note:            	Should call "DListDestroy()" at end of use.
*******************************************************************************/
dlist_t *DListCreate(void)		/* canceled malloc for dummys */
{
	dlist_t *list = (dlist_t *)malloc(sizeof(dlist_t));
	
	if(NULL == list)
	{
		return NULL;
	}	
	
	/* set tail next and prev */
	list->tail.next = NULL;
	list->head.prev = NULL;
	
	/* set tail next and prev */
	list->head.next = &list->tail;
	list->tail.prev = &list->head;	
	
	

	return list;
}

/*******************************************************************************
Description:     	Deletes a list pointed by "list" from memory.
Time Complexity: 	Determined by system call complexity. 
Notes:           	Undefined behaviour if list is NULL.
*******************************************************************************/
void DListDestroy(dlist_t *list)
{
	dnode_t *current = NULL;
	dnode_t *tmp = NULL;
	
	assert(NULL != list);
	
	/* initialaize current pointer to start */
	current = list->head.next;
	
	/* run with current till the end and free all the nodes */
	while (current != &list->tail)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}

	free(list);
	list = NULL;
}

/*******************************************************************************
Description:     	Insert 'data' before 'where' in list pointed to by 'list'.
Return value:    	Iterator to inserted data in success otherwise
					DListIterEnd().   
Time Complexity: 	O(1) + system call complexity.
Notes: 			 	Undefined behaviour if list is invalid pointer,
          			Undefined behaviour if 'where' is out of list's range.
*******************************************************************************/
ditr_t DListInsertBefore(dlist_t *list, ditr_t where, void *data)
{
	dnode_t *where_node = NULL;
	dnode_t *new_node = NULL;
	dnode_t *where_prev_node = NULL;

	assert(NULL != list);
    
    new_node = (dnode_t *)malloc(sizeof(dnode_t));
	
	if(NULL == new_node)
    {
        return NODE_TO_ITR(DListIterEnd(list));
    }
    
	where_node = ITR_TO_NODE(where);
	where_prev_node = DListIterPrev(where_node);
    
	/* new node :set the data we get from user and point next to where and
	   new node prev = prev of where */
	new_node->data = data;
	new_node->next = where_node;	
	new_node->prev = where_prev_node;	
	
	/* update next/prev of where and where->preve to point to current*/
	where_prev_node->next = new_node;
	where_node->prev = new_node;
	
	return NODE_TO_ITR(new_node);
}

/*******************************************************************************
Description:     	Deletes the element from the list pointed to by "what".
Return value:    	Iterator to next element.
Time Complexity: 	O(1) + system call complexity.
Notes:           	Undefined behaviour if list is invalid pointer,
					Undefined behaviour if "what" is out of lists range or
                 	if "what" is dummy node (DListEnd()). 
*******************************************************************************/
ditr_t DListRemove(ditr_t what)
{
	dnode_t *what_node = NULL;
	dnode_t *node_next = NULL;
	dnode_t *nude_prev = NULL;	
		
	/* node_next = what->next , nude_prev = what->prev */
	what_node = ITR_TO_NODE(what);
	node_next = DListIterNext(what_node);
	nude_prev = DListIterPrev(what_node);	
	
	/* update next/prev of node_next and nude_prev */
	node_next->prev = nude_prev;
	nude_prev->next = node_next;
	
	/*free what node and set to NULL*/
	free(what_node);
	what_node = NULL;
	
	return NODE_TO_ITR(node_next);
}

/*******************************************************************************
Description:     	Returns number of elements in "list".
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
size_t DListSize(const dlist_t *list)
{
	size_t size_list = 0;
	ditr_t count_size = NULL;
	ditr_t end = NULL;
	
	assert(NULL != list);
	
	count_size = DListIterBegin(list);
	end = DListIterEnd(list);
		
	while (count_size != end)
	{
		count_size = DListIterNext(count_size);
		++size_list;
	}

	return size_list;
}

/*******************************************************************************
Description: 		Checks if "list" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
int DListIsEmpty(const dlist_t *list)
{
	assert(NULL != list);	
	
	/* if head next == tail its empty */
	return (list->head.next == &list->tail);
								/* & */ /* begin + end , malloc create*/
}

/*******************************************************************************
Description:     	Insert 'data' at the end of the list pointed to by 'list'.
Return value:    	Iterator to inserted data in success, otherwise 
					DListIterEnd().          
Time Complexity: 	O(1) + system call complexity
Notes:           	Undefined behaviour if 'where' is out of list's range,
				 	undefined behaviour if list is invalid pointer.
*******************************************************************************/
ditr_t DListPushBack(dlist_t *list, void *data)
{
	assert(NULL != list);	
	
	return DListInsertBefore(list,&list->tail , data);
}

/*******************************************************************************
Description:     	Insert 'data' at the beginning of the list pointed to by 
					'list'.
Return value:    	Iterator to inserted data in success, otherwise 
					DListIterEnd().           
Time Complexity: 	O(1) + system call complexity.
Notes:           	Undefined behaviour if 'where' is out of lists range,
				 	undefined behaviour if list is invalid pointer.
*******************************************************************************/
ditr_t DListPushFront(dlist_t *list, void *data)
{
	assert(NULL != list);	
	
	return DListInsertBefore(list, DListIterBegin(list), data);
}

/*******************************************************************************
Description:    	Deletes the element from the end of the list pointed to by 
					"list".
Return value:    	The data of the deleted iterator.
Time Complexity: 	O(1) + system call complexity.
Notes: 				Undefined behaviour if list is invalid pointer
		         	Undefined behaviour if list is empty.
*******************************************************************************/
void *DListPopBack(dlist_t *list)
{
	void *data = DListGetData(DListIterPrev(DListIterEnd(list)));
	
	assert(NULL != list);	
		
	DListRemove(DListIterPrev(DListIterEnd(list)));
	
	return data;
}

/*******************************************************************************
Description:    	Deletes the element from the beginning of the list pointed 
					out by "list".
Return value:    	The data of the deleted iterator.
Time Complexity: 	O(1) + system call complexity.
Notes: 				Undefined behaviour if list is invalid pointer
		         	Undefined behaviour if list is empty.      
*******************************************************************************/
void *DListPopFront(dlist_t *list)
{
	void *data = DListGetData(DListIterBegin(list));
	
	assert(NULL != list);
			
	DListRemove(DListIterBegin(list));
	
	return data;
}

/*******************************************************************************
Description:  	  	Calls "action_func" on each element between [from, to)
					("to" is exclusive) until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n)
Notes:            	Undefined behavior if "from" or "to" are not valid or do
					not belong to the same list.
*******************************************************************************/
int DListForEach(ditr_t from, ditr_t to, action_func_t action_func,
				 void *user_params)
{
	assert(NULL != action_func);
	assert(NULL != user_params);
	
	while (from != to)
	{
		if ( !action_func(DListGetData(from), user_params) )
		{
			return 1;
		}
		
		from = DListIterNext(from);
	}
	
	return 0;
}

/*******************************************************************************
Description:  		Insert the sublist that begins with "src_first" and ends
                    with "src_last" before the iterator "dest_where" 
Return value:     	Iterator to the last inserted  element
Time complexity:  	O(1)
Notes:            	Undefined behaviour if "from" or "to" are not valid or do 
                    not belong to the same list.
*******************************************************************************/
ditr_t DListSplice(ditr_t dest_where, ditr_t src_first, ditr_t src_last)
{
	dnode_t *dest_prev = NULL, *dest_curr= NULL, *src_prev = NULL;
	dnode_t *src_next = NULL, *first = NULL, *last = NULL;
	
	assert(NULL != dest_where);
	assert(NULL != src_first);
	assert(NULL != src_last);
	
	/*get the previous node of the iterator where it will be spliced*/
	dest_prev = ITR_TO_NODE(DListIterPrev(dest_where));
	dest_curr = ITR_TO_NODE(dest_where);
	
	/*get the previous node of the first iterator that will be spliced with
	source list*/
	src_prev = ITR_TO_NODE(DListIterPrev(src_first));
	
	/*get the next node of the last iterator that will be spliced with
	source list*/
	src_next = ITR_TO_NODE(DListIterNext(src_last));
	
	first = ITR_TO_NODE(src_first);
	last = ITR_TO_NODE(src_last);
	
	src_prev->next = src_next;
	src_next->prev = src_prev;
	
	dest_prev->next = first;
	first->prev = dest_prev;
	
	dest_curr->prev = last;
	last->next = dest_curr;
	
	
	return src_last;
}

/*******************************************************************************
Description:		Finds the first match according to "match_func" in the range
					[from, to) ( "to" is exclusive).
Return value:   	Iterator to first match or "to" if not found
Time complexity:    O(n)
Note:          		Undefined behaviour if "from" or "to" are not valid or do
					not belong to the same list or if match_func is NULL
*******************************************************************************/
ditr_t DListFind(ditr_t from, ditr_t to, is_match_func_t match_func, 
				void *user_params)
{
	assert(NULL != from);
	assert(NULL != to);
	
	while (from != to)
	{
		if ( match_func(DListGetData(from), user_params) )
		{
			return from;
		}
		
		from = DListIterNext(from);
	}
	
	return from;
}

/*******************************************************************************
Description:		Finds all matches according to "match_func" in the range 
					[from, to) ("to" is exclusive).
Return value:   	0 for success, 1 for fail. 
Time complexity:    O(n) + system call complexity
Note:          		Undefined behaviour if "from" or "to" are not valid or do
					not belong to the same list or if match_func is NULL
                    
*******************************************************************************/
int DListMultiFind(ditr_t from, ditr_t to, is_match_func_t match_func, 
				void *user_params, dlist_t *results_list)
{
    size_t counter = 0;

    assert(NULL != ITR_TO_NODE(from));
    assert(NULL != ITR_TO_NODE(to));
    assert(NULL != match_func);
    assert(NULL != user_params);
    assert(NULL != results_list);

    while(!DListIterIsEqual(from, to))
    {
        if(1 == (*match_func)(DListGetData(from), user_params))
        {
            if(DListIterIsEqual(DListIterEnd(results_list),
               DListPushBack(results_list, DListGetData(from))))
            {
                while(counter)
                {
                    DListPopBack(results_list);
                    --counter;
                }

                return 1;
            }

            ++counter;
        }
        
        from = DListIterNext(from);
    }

    return 0;
}
/*******************************************************************************
Description:  		Returns next iterator of "itr".
Time complexity:  	O(1)
Note:    			Undefined behaviour if "itr" is Dummy End (DListIterEnd()).
*******************************************************************************/
ditr_t DListIterNext(ditr_t itr)
{
	return NODE_TO_ITR( ITR_TO_NODE(itr)->next );
}

/*******************************************************************************
Description:  		Returns the iterator before "itr".
Time complexity:  	O(1)
Note:    			Undefined behaviour if "itr" is DListBegin() 
*******************************************************************************/
ditr_t DListIterPrev(ditr_t itr)
{
	return NODE_TO_ITR( ITR_TO_NODE(itr)->prev );
}

/*******************************************************************************
Description: 		Returns an iterator to the first valid elem (invalid element).   
Time complexity:  	O(1)
Note:        		Undefined behaviour if list is invalid pointer
*******************************************************************************/
ditr_t DListIterBegin(const dlist_t *list)
{
	assert(NULL != list);
	
	return NODE_TO_ITR(list->head.next);
}

/*******************************************************************************
Description:   		Returns an iterator to the dummy end (invalid element) 
Time complexity:  	O(1)
Note: 				Undefined behaviour if "list" is invalid pointer
*******************************************************************************/
ditr_t DListIterEnd(const dlist_t *list)
{
	assert(NULL != list);
	
	return NODE_TO_ITR(&list->tail);
}

/*******************************************************************************
Description: 		Checks if iter1 and iter2 are iterators for the same 
					element.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
*******************************************************************************/
int DListIterIsEqual(ditr_t iter1, ditr_t iter2)
{
{
	return  ITR_TO_NODE(iter1) == ITR_TO_NODE(iter2)? 1 : 0;
}
}

/*******************************************************************************
description:     	Returns pointer to data at 'iter'.
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if 'itr' is out of lists range.
*******************************************************************************/
void *DListGetData(const ditr_t itr)
{
	return ITR_TO_NODE(itr)->data;
}
















