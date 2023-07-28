/*******************************************************************************
****** Author: 		Daniel
****** Reviewer:    Marina	
****** Date: 		21.06.2023
****** Version:		1
*******************************************************************************/

/**********************		Headers		***************************************/
#include <stdlib.h>  /* malloc, free */
#include <stddef.h>  /* size_t 		 */
#include <assert.h>	 /* assert		 */

#include "sorted_list.h"   /* ditr_t */ 

/******************************************************************************/
struct sorted_list
{
	dlist_t 	    *srtlist;
	sort_func_t 	sort_func;
};

typedef struct user_params
{
	void *params;
	sort_func_t sort_func;
}user_params_t;


int ConvertSortToIsMatch(const void *data, const void *user_params)
{
	user_params_t *us_pa_struct = (user_params_t *)user_params;
	
	return !us_pa_struct->sort_func(data, us_pa_struct->params);
}

/* Function convert srtitr To ditr */
static ditr_t SrtToD(srtitr_t srt_itr)
{
	
	return srt_itr.iterator;
}

/* Function convert ditr To srtitr */
static srtitr_t DToSrt(ditr_t itr)
{
	srtitr_t srt_itr;
	srt_itr.iterator = itr;
	
	return srt_itr;
}

/******************************************************************************
Description:     	Creates an empty sorted list sorted by 'sort_func'.
Return value:    	Pointer to list in case of success, otherwise NULL.
Time Complexity: 	Determined by system call complexity. 
Note:            	Should call "SrtListDestroy()" at end of use.
					User must be aware that created list sorted only by 
					'sort_func'
******************************************************************************/
srtlist_t *SrtListCreate(sort_func_t sort_func)
{
	
	srtlist_t *list = (srtlist_t *)malloc(sizeof(srtlist_t));
	
	/* if allocation fail for list */ 
	if(NULL == list)
	{
		return NULL;
	}
	
	list->srtlist = DListCreate();
	
	/* if allocation fail for DListCreate we need to free list */ 
	if(NULL == list->srtlist)
	{
		free(list);
		list = NULL;
		return NULL;
	}
	
	list->sort_func = sort_func;
	
	return list;
}

/******************************************************************************
Description:     	Deletes a list pointed by "list" from memory.
Time Complexity: 	Determined by system call complexity. 
Notes:           	Undefined behaviour if list is NULL.
*******************************************************************************/
void SrtListDestroy(srtlist_t *list)
{ 
	DListDestroy(list->srtlist);
	free(list);
}

/*******************************************************************************
Description:     	Inserts 'data' to the correct place in 'list' according to 
					'sort_func'provided by user at 'SrtListCreate' 
Return value:    	Iterator to inserted data on success otherwise
					SrtListIterEnd().   
Time Complexity: 	O(n) + system call complexity.
Notes: 			 	Undefined behaviour if list is invalid pointer.
*******************************************************************************/
srtitr_t SrtListInsert(srtlist_t *list, void *data)
{
	ditr_t itr = NULL;
	assert(list);
	itr = DListIterBegin(list->srtlist);
	
	while(!DListIterIsEqual(itr , DListIterEnd(list->srtlist)))
	{
		
		if(list->sort_func( DListGetData(itr) , data) >= 0)
		{
			return DToSrt(DListInsertBefore(list->srtlist ,  itr, data));
		}
	
		itr = DListIterNext(itr);
	
	}
	
	return DToSrt(DListPushBack(list->srtlist,data));

}

/*******************************************************************************
Description:     	Deletes the element from the list pointed to by "what".
Return value:    	Iterator to next element.
Time Complexity: 	O(1) + system call complexity.
Notes:         		Undefined behaviour if "what" is out of list's 
                    range or SrtListIterEnd() 
*******************************************************************************/
srtitr_t SrtListRemove(srtitr_t what)
{
	return DToSrt(DListRemove(SrtToD(what)));
}

/*******************************************************************************
Description:     	Returns number of elements in "list".
Time Complexity: 	O(n)
Notes:			 	Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
size_t SrtListSize(const srtlist_t *list)
{
	assert(NULL != list->srtlist);
	
	return DListSize(list->srtlist);
}

/*******************************************************************************
Description: 		Checks if "list" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
int SrtListIsEmpty(const srtlist_t *list)
{
	assert(NULL != list->srtlist);	
	
	return DListIsEmpty(list->srtlist);			
}

/*******************************************************************************
Description:    	Deletes the element from the end of the list pointed to by 
					"list".
Return value:    	The data of the deleted iterator.
Time Complexity: 	O(1) + system call complexity.
Notes: 				Undefined behaviour if list is invalid pointer
		         	Undefined behaviour if list is empty.
*******************************************************************************/
void *SrtListPopBack(srtlist_t *list)
{
	assert(NULL != list->srtlist);	
	
	return DListPopBack(list->srtlist);		
}

/*******************************************************************************
Description:    	Deletes the element from the beginning of the list pointed 
					out by "list".
Return value:    	The data of the deleted iterator.
Time Complexity: 	O(1) + system call complexity.
Notes: 				Undefined behaviour if list is invalid pointer
		         	Undefined behaviour if list is empty.      
*******************************************************************************/
void *SrtListPopFront(srtlist_t *list)
{
	assert(NULL != list->srtlist);	
	
	return DListPopFront(list->srtlist);
}


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
				 void *user_paramss)
{
	return DListForEach(SrtToD(from) , SrtToD(to), action_func, user_paramss);
}

/*******************************************************************************
Description:		Finds the first match according to "sort_func" 
					(sorting criteria) provided by user at 'SrtListCreate',
                    in the range  [from, to) ("to" is exclusive).
Return value:   	Iterator to first match in list if found, otherwise "to".
Time complexity:    O(n)
Note:          		Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
srtitr_t SrtListFind(const srtlist_t *list, srtitr_t from, srtitr_t to, void *data)
{
	user_params_t us_pa_struct;
	
	assert(NULL != list->srtlist);	
	
	us_pa_struct.params = data;
	us_pa_struct.sort_func = list->sort_func;
 
	return DToSrt( DListFind(SrtToD(from) , SrtToD(to), ConvertSortToIsMatch, &us_pa_struct) );
}

/*******************************************************************************
Description:		Finds first match according to "match_func" in the range 
					[from, to) ("to" is exclusive).
Return value:   	Iterator to first match  if found , otherwise "to" 
Time complexity:    O(n) + system call complexity
Note:          		Undefined behaviour if "from" or "to" are not valid or do
					not belong to the same list or if match_func is NULL
*******************************************************************************/
srtitr_t SrtListFindIf(srtitr_t from, srtitr_t to, is_match_func_t match_func, 
				void *user_params)
{
	return DToSrt( DListFind(SrtToD(from) , SrtToD(to), match_func, user_params) );
}

/*******************************************************************************
Description:  		Merges dest_list and src_list into dest_list. src_list
                    should remain empty.
Time complexity:  	O(n + m)
Notes:            	Undefined behaviour if lists are sorted according to different 
                    sorting criteria, or pointers are not valid
*******************************************************************************/
void SortListMerge(srtlist_t *dest_list, srtlist_t *src_list)
{
	int is_splice = 0;
	srtitr_t dest_itr;
	srtitr_t src_last;
	srtitr_t src_first;
	
	assert(dest_list);
	assert(src_list);
	
	dest_itr = SrtListIterBegin(dest_list);
	src_last = SrtListIterBegin(src_list);

	while( !SrtListIterIsEqual(dest_itr ,SrtListIterEnd(dest_list)) )
	{
		
		src_first = src_last;
				
		while(!SrtListIterIsEqual(src_last ,SrtListIterEnd(src_list)) && 
		dest_list->sort_func(SrtListGetData(dest_itr),SrtListGetData(src_last)) >= 0)
		{
			src_last = SrtListIterNext(src_last);
			is_splice = 1;
		}
		
		if(is_splice)
		{
			DListSplice(SrtToD(dest_itr), SrtToD(src_first), SrtToD(SrtListIterPrev(src_last)));
		}
		
		is_splice = 0;
		dest_itr = SrtListIterNext(dest_itr);
	
	}
	
}

/*******************************************************************************
Description:  		Returns next iterator of "itr".
Time complexity:  	O(1)
Note:    			Undefined behaviour if "itr" is Dummy End (SrtListIterEnd()).
*******************************************************************************/
srtitr_t SrtListIterNext(srtitr_t itr)
{ 	
	itr.iterator = DListIterNext( SrtToD(itr) );
	
	return itr;	
}

/*******************************************************************************
Description:  		Returns the iterator before "itr".
Time complexity:  	O(1)
Note:    			Undefined behaviour if "itr" is SrtListBegin() 
*******************************************************************************/
srtitr_t SrtListIterPrev(srtitr_t itr)
{	
	itr.iterator = DListIterPrev( SrtToD(itr) );
	
	return itr;		
}

/*******************************************************************************
Description: 		Returns an iterator to the first valid elem (nvalid element).   
Time complexity:  	O(1)
Note:        		Undefined behaviour if list is invalid pointer
*******************************************************************************/
srtitr_t SrtListIterBegin(const srtlist_t *list)
{
	srtitr_t srt_itr;
	
	assert(list->srtlist);	
		
	srt_itr.iterator = DListIterBegin(list->srtlist);
	
	return srt_itr;	
}

/*******************************************************************************
Description:   		Returns an iterator to the dummy end (invalid element) 
Time complexity:  	O(1)
Note: 				Undefined behaviour if "list" is invalid pointer
*******************************************************************************/
srtitr_t SrtListIterEnd(const srtlist_t *list)
{
	srtitr_t srt_itr;
	
	assert(NULL != list->srtlist);	
		
	srt_itr.iterator = DListIterEnd(list->srtlist);
	
	return srt_itr;	
}

/*******************************************************************************
Description: 		Checks if iter1 and iter2 are iterators to the same 
					element.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
*******************************************************************************/
int SrtListIterIsEqual(srtitr_t iter1, srtitr_t iter2)
{
	return DListIterIsEqual( SrtToD(iter1), SrtToD(iter2) )	;
}

/*******************************************************************************
description:     	Returns pointer to data at 'iter'.
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if 'itr' is out of lists range.
*******************************************************************************/
void *SrtListGetData(const srtitr_t itr)
{
	return DListGetData( SrtToD(itr) );	
}



