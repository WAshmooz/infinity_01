/*******************************************************************************
****** Author: HRD31
****** Date: 31.07.2023
****** Version: 1
****** Description: Data structure that maps keys to values.
*******************************************************************************/
#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h>    /* size_t        */
#include "aux_funcs.h" /* action_func_t */

typedef struct hash hash_t;

/******************************************************************************
Description:     	Converts a given 'key' into index in range [0, table_size).
Return value:    	index related to 'key'.

Notes: 				Number of collisions should be less while placing the record
					in the hash table.no collision should occur.
					Hash function should produce such keys which will get 
					distributed uniformly over an array.
*******************************************************************************/
typedef size_t (*hash_func_t)(const void *key);
 
/******************************************************************************
Description:     	Creates hash table ordered according to "hash_func".
Return value:    	Pointer to hash table in case of success, otherwise NULL.
Time Complexity: 	O(size).
Note:            	Should call "HashDestroy()" at end of use
                    'table_size should be big enough to store hash_func values
******************************************************************************/
hash_t *HashCreate(size_t table_size, hash_func_t hash_func, is_match_func_t match);


/******************************************************************************
Description:     	Deletes a hash table pointed to by "hash" from memory.
Time Complexity: 	O(n). 
Notes:           	Undefined behaviour if hash is NULL.
*******************************************************************************/
void HashDestroy(hash_t *hash);


/*******************************************************************************
Description:     	Deletes the element related to 'key' from hash table.
Time Complexity: 	O(1) average, O(n) worst case.
Notes               Undefined behaviour if hash is invalid pointer.
					undefined behaviour if key is invalid pointer.
*******************************************************************************/
void HashRemove(hash_t *hash, const void *key);


/*******************************************************************************
Description:     	Inserts 'val' to the correct place in 'hash' according to 
					'hash_func' provided by user at 'HashCreate' 
Return value:    	0 in case of success otherwise 1.  
Time Complexity: 	O(1) average, O(n) worst case.
Notes: 			 	Undefined behaviour if hash is invalid pointer.
					Undefined behaviour if key is invalid pointer.
*******************************************************************************/
int HashInsert(hash_t *hash,const void *key, void *val);


/*******************************************************************************
Description:     	Returns number of elements in "hash".
Time Complexity: 	O(n).
Notes:			 	Undefined behaviour if 'hash' is invalid pointer.
*******************************************************************************/
size_t HashSize(const hash_t *hash);


/*******************************************************************************
Description: 		Checks if "hash" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(n)
Note: 				Undefined behaviour if 'hash' is invalid pointer.
*******************************************************************************/
int HashIsEmpty(const hash_t *hash);


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

#endif    /*__HASH_H__*/
