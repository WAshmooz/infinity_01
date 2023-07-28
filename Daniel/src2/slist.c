/*******************************************************************************
****** Author: 		Daniel
****** Reviewer: 	Rostic
****** Date: 		14.06.2023
****** Version:		2
*******************************************************************************/

/**********************		Headers		***************************************/

#include <stdio.h>  /* size_t  */
#include <stdlib.h> /* malloc, free */ 
#include <assert.h> /* assert  */ 
#include "slist.h"

#define ITR_TO_NODE(itr) ( (snode_t *)itr )
#define NODE_TO_ITR(node)  ( (sitr_t)node )
/******************************************************************************/
struct slist
{
	snode_t *head;
	snode_t *tail;
};

struct snode
{
	void *data;		
	snode_t *next;			
};

/********************************************************************************/
slist_t *SListCreate(void)
{
	slist_t *slist = (slist_t *)malloc(sizeof(slist_t));
	snode_t *dummy;
	
	if(NULL == slist)
	{
		return NULL;
	}
	
	dummy = (snode_t *)malloc(sizeof(snode_t));
		
	if(NULL == dummy)
	{
		free(slist);
		slist = NULL;
		return NULL;
	}
	
	/* initialize dummy data point to list so we can get access
	   and dumyy next point to NULL (end) */ 
	dummy->data = (void *)slist;
	dummy->next = NULL;
	
	/* head and list are pointing to dummy (there is only one node (dummy)) */ 
	slist->head = dummy;
	slist->tail = dummy;	

	return slist;
}

/********************************************************************************/
void SListDestroy(slist_t *list)
{
	snode_t *current = NULL;
	snode_t *tmp = NULL;
	
	assert(NULL != list);
	
	/* initialaize current pointer to start */
	current = list->head;
	
	/* run with currente till the end and free all the nodes */
	while (current != NULL)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}

	free(list);
	list = NULL;
}

/********************************************************************************/
sitr_t SListInsertBefore(slist_t *list, sitr_t where, void *data)
{
	snode_t *where_node = NULL;
	snode_t *current_node = NULL;
	
	assert(NULL != list);
	assert(NULL != where);
	
	where_node = ITR_TO_NODE(where);
	current_node = (snode_t *)malloc(sizeof(snode_t));
	
	if(NULL == current_node)
    {
        return NODE_TO_ITR(list->tail);;
    }

	/* if we insert before tail we need to intioalize the tail */
	if (list->tail == where_node)
	{
		list->tail = current_node;	
	}
	
	/* initialize current_node */
	current_node->data = where_node->data;
	current_node->next = where_node->next;
	
	where_node->data = data;
	where_node->next = current_node;
	
	return where;
}

/********************************************************************************/
sitr_t SlistRemove(sitr_t what)
{
	snode_t *what_node =NULL;
	snode_t *tmp_node = NULL;
	
	what_node = ITR_TO_NODE(what);
	/* copy next node to tmp*/ 
	tmp_node = what_node->next; 
	/*update node to the tmp (the next node)*/
	what_node->data = tmp_node->data;
	what_node->next = tmp_node->next;
	/*remove the next node after we swap the data node*/
	free(tmp_node);
	tmp_node = NULL;
	
	/*update tail if we remobe the last node*/
	if (what_node->next == NULL)
	{
		( (slist_t *)(what_node->data))->tail = what_node;	
	}
	
	return what;
}

/********************************************************************************/
size_t SListSize(const slist_t *list)
{
	size_t size_list = 0;
	snode_t *tmp_node = NULL;
	snode_t *end = NULL;
	
	assert(NULL != list);
	
	tmp_node = list->head;
	end = list->tail;
		
	while (tmp_node != end)
	{
		tmp_node = tmp_node->next;
		++size_list;
	}

	return size_list;
}
/********************************************************************************/
void *SListGetData(sitr_t iter)
{
	return ITR_TO_NODE(iter)->data;
}

/********************************************************************************/
void SListSetData(sitr_t itr, void *new_data)
{
	assert(NULL != new_data);
	
	ITR_TO_NODE(itr)->data = new_data;
}

/********************************************************************************/
sitr_t SListIterNext(sitr_t itr)
{
	return NODE_TO_ITR( ITR_TO_NODE(itr)->next );
}

/********************************************************************************/
sitr_t SListIterBegin(const slist_t *list)
{
	assert(NULL != list);
	
	return NODE_TO_ITR(list->head);
}

/********************************************************************************/
sitr_t SListIterEnd(const slist_t *list)
{
	assert(NULL != list);
	
	return NODE_TO_ITR(list->tail);
}

/********************************************************************************/
int SListIterIsEqual(sitr_t iter1, sitr_t iter2)
{
	return  ITR_TO_NODE(iter1) == ITR_TO_NODE(iter2) ;
}

/********************************************************************************/

int SListForEach(sitr_t from, sitr_t to, action_func_t action_func,
				 void *user_params)
{

	snode_t *tmp_node = NULL;
		
	assert(NULL != action_func);
	assert(NULL != user_params);
	
	tmp_node = ITR_TO_NODE(from);
	
	while (tmp_node != ITR_TO_NODE(to))
	{
		if ( !action_func(tmp_node->data, user_params) )
		{
			return 1;
		}
		
		tmp_node = tmp_node->next;
	}
	
	return 0;
}

/********************************************************************************/
sitr_t SListFind(sitr_t from, sitr_t to, 
						is_match_func_t match_func, void *user_params)
{
	snode_t *tmp_cmp = NULL;
		
	assert(NULL != from);
	assert(NULL != to);
	
	tmp_cmp = ITR_TO_NODE(from);
	
	while (tmp_cmp != ITR_TO_NODE(to))
	{
		if ( match_func(tmp_cmp->data, user_params) )
		{
			return NODE_TO_ITR(tmp_cmp);
		}
		
		tmp_cmp = tmp_cmp->next;
	}
	
	return NODE_TO_ITR(tmp_cmp);
}

/********************************************************************************/
slist_t *SListAppend(slist_t *list1, slist_t *list2)
{	
	assert(NULL != list1);
	assert(NULL != list2);
	
	/* check if size of list2 != 0, function can deal with size list1 = 0 */
	if (NULL == list2->head->next)
	{
		return list1;
	}

	/* initialize dummy1 in list1 to first node of list2 */
	list1->tail->data = list2->head->data;
	list1->tail->next = list2->head->next;
	
	/* initialize first node in list2 to tail of list2 */
	list2->head->data = list2->tail->data;
	list2->head->next = list2->tail->next;	/* list2->tail->next = NULL*/
	
	/* initialize tail+data in list1 to be tail of list2 + data(pointer to list1)*/
	list2->tail->data = list1;  
	list1->tail = list2->tail;
	
	/* initialize tail in list2 to point head of list2*/
	list2->tail = list2->head;
	
	return list1;
}




























