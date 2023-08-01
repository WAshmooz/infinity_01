/*******************************************************************************
****** Author: HRD31
****** Date: 31.07.2023
****** Version: 1
****** Description: Data structure that maps keys to values.
*******************************************************************************/
#include <stddef.h> /* size_t       */
#include <stdlib.h> /* malloc, free */ 
#include <assert.h> /* assert  		*/ 
#include "dlist.h"
#include "hash.h" 

#define DEBUG
#ifdef DEBUG
	#include <stdio.h>
#endif

#define UNUSED(x) (void)(x)

#define SUCCESS 0
#define FAIL 	1
/*****************************************************************************/

struct hash
{
	is_match_func_t match_func;
    hash_func_t hash_func;
	dlist_t **table;
	size_t table_size;	/* num of ceels/pointers */
};


typedef struct hash_elem
{
	const void *key;
	void *val;
}hash_elem_t;


/******************************************************************************
Description:     	Creates hash table ordered according to "hash_func".
Return value:    	Pointer to hash table in case of success, otherwise NULL.
Time Complexity: 	O(size).
Note:            	Should call "HashDestroy()" at end of use
                    'table_size should be big enough to store hash_func values
******************************************************************************/
hash_t *HashCreate(size_t table_size, hash_func_t hash_func, is_match_func_t match)
{
	dlist_t **table = NULL;
	size_t i = 0, j = 0;
	
	hash_t *hash = (hash_t *)malloc(sizeof(hash_t));
	if(NULL == hash)
	{
		return NULL;
	}
	
	assert(NULL != hash_func);
	assert(NULL != match);	
	
	table = (dlist_t **)malloc(sizeof(dlist_t *) * table_size);
	if(NULL == table)
	{
		free(hash);
		hash = NULL;
		return NULL;
	}

	hash->table = table;
	hash->table_size = table_size;
	
	for (i = 0; i < table_size; i++)
	{
		hash->table[i] = DListCreate();
		
		if(NULL == hash->table[i])
		{
			for (j = 0; j < i; j++)
			{
				DListDestroy(hash->table[i]);
				hash->table[i] = NULL;
			}
			
			free(table); 	table = NULL;
			free(hash);		hash = NULL;


			return NULL;
		}
	}
		
	hash->match_func = match;
	hash->hash_func = hash_func;

		
	return hash;	
}

/******************************************************************************
Description:     	Deletes a hash table pointed to by "hash" from memory.
Time Complexity: 	O(n). 
Notes:           	Undefined behaviour if hash is NULL.
*******************************************************************************/
static int FreeData(void *data, void* unused_param)
{
	UNUSED(unused_param);
	free((hash_elem_t *)data);
	data = NULL;
	
	return 0;
}

void HashDestroy(hash_t *hash)
{
	size_t i = 0;
	int unused_param = 9;
				
	assert(NULL != hash);
	
	for (i = 0; i < hash->table_size; i++)
	{
		DListForEach(DListIterBegin(hash->table[i]), DListIterEnd(hash->table[i]),
					 FreeData, &unused_param);
		DListDestroy(hash->table[i]);
		hash->table[i] = NULL;
	}

	free(hash->table); 	hash->table = NULL;
	free(hash);			hash = NULL;
}

/*******************************************************************************
Description:     	Deletes the element related to 'key' from hash table.
Time Complexity: 	O(1) average, O(n) worst case.
Notes               Undefined behaviour if hash is invalid pointer.
					undefined behaviour if key is invalid pointer.
*******************************************************************************/
static ditr_t DListFindForHash(ditr_t from, ditr_t to, is_match_func_t match_func, 
						void *key)
{
	hash_elem_t *get_data = NULL;
	assert(NULL != from);
	assert(NULL != to);
	
	while (!DListIterIsEqual(from, to))
	{
		get_data = (hash_elem_t *)DListGetData(from);
		
		if ( match_func(get_data->key, key) )
		{
			return from;
		}
		
		from = DListIterNext(from);
	}
	
	return from;
}


void HashRemove(hash_t *hash, const void *key)
{
	ditr_t itr_to_remove = NULL;
	dlist_t *list = NULL;
	hash_elem_t *hash_elem;
	hash_elem->key = key;
	
	assert(NULL != hash);	
	assert(NULL != key);	

	list = hash->table[hash->hash_func(key)];
	itr_to_remove = DListFindForHash(DListIterBegin(list), DListIterEnd(list), 
							hash->match_func, hash_elem);
	
	if ( NULL == itr_to_remove)
	{
		DListRemove(itr_to_remove);
	}
}

/*******************************************************************************
Description:     	Inserts 'val' to the correct place in 'hash' according to 
					'hash_func' provided by user at 'HashCreate' 
Return value:    	0 in case of success otherwise 1.  
Time Complexity: 	O(1) average, O(n) worst case.
Notes: 			 	Undefined behaviour if hash is invalid pointer.
					Undefined behaviour if key is invalid pointer.
*******************************************************************************/
int HashInsert(hash_t *hash,const void *key, void *val)
{
	dlist_t *list = NULL;
	ditr_t check_insert = NULL;	
	hash_elem_t *hash_elem = (hash_elem_t *)malloc(sizeof(hash_elem_t));
	if(NULL == hash_elem)
	{
		return FAIL;
	}
	
	assert(NULL != hash);
	assert(NULL != key);
	
	list = hash->table[hash->hash_func(key)]; /*pointer to the relevant cell*/
	hash_elem->key = key;
	hash_elem->val = val;
	
	check_insert = DListPushFront(list, hash_elem);
	
	if(DListIterIsEqual(DListIterEnd(list), check_insert))
	{
		return FAIL;
	}
	
	return SUCCESS;
}

/*******************************************************************************
Description:     	Returns number of elements in "hash".
Time Complexity: 	O(n).
Notes:			 	Undefined behaviour if 'hash' is invalid pointer.
*******************************************************************************/
size_t HashSize(const hash_t *hash)
{
	size_t i = 0;
	size_t hash_size = 0;	
	
	assert(NULL != hash);	
	
	for (i = 0; i < hash->table_size; i++)
	{
		hash_size += DListSize(hash->table[i]);
	}
	
	return 	hash_size;
}

/*******************************************************************************
Description: 		Checks if "hash" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(n)
Note: 				Undefined behaviour if 'hash' is invalid pointer.
*******************************************************************************/
int HashIsEmpty(const hash_t *hash)
{
	return (0 == HashSize(hash)? 1 : 0);
}
/*******************************************************************************
Description:		Finds 'val' mapped to 'key'.
Return value:       Pointer to val in case of success, otherwise NULL.
Time complexity:    O(1) average, O(n) worst case.
Note:          		Undefined behaviour if 'hash' is invalid pointer.
*******************************************************************************/
void *HashFind(const hash_t *hash, const void *key);


/*******************************************************************************
Description:  	  	Calls "action_func" on all elements in hash until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n)
Notes:            	Undefined behaviour if 'hash' is invalid pointer.
*******************************************************************************/
int HashForEach(hash_t *hash, action_func_t action_func, void *user_params);


