/*******************************************************************************
****** Author: HRD31
****** Date: 06.07.2023
****** Version: 2.1
****** Description: fixed sized allocator ds header.
*******************************************************************************/
#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h>  /* size_t */

typedef struct fsa fsa_t;

/*******************************************************************************
Description:     	Calculate Memory Pool Size 
Return value:    	size of memory pool.
Time Complexity: 	O(1).
notes:              If 'block_size' < sizeof(size_t) then 
                    block_size = sizeof(size_t).
*******************************************************************************/
size_t FsaSuggestSize(size_t num_of_block, size_t block_size);


/*******************************************************************************
Description:     	Initializes Memory managment.  
Return value:    	Pointer to memory pool in case of success, otherwise NULL. 
Time Complexity: 	O(n)
notes:              Should call 'FsaSuggestSize()' to calculate 'pool_size'.
*******************************************************************************/
fsa_t *FsaInit (void *memory, size_t pool_size, size_t block_size);


/*******************************************************************************
Description:     	Return pointer to memory Block
Return value:    	Pointer to memory block in case of success, otherwise NULL        
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
*******************************************************************************/
void *FsaAlloc(fsa_t *m_pool);
    

/*******************************************************************************
Description:     	Free block of memory pointed by "m_block".
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
                    Undefined behaviour if 'm_block' is invalid pointer.
                    Undefined behaviour if 'm_block' has been freed already.
*******************************************************************************/
void FsaFree(fsa_t *m_pool, void *m_block);


/*******************************************************************************
Description:     	Return Numbers of free blocks.  
Time Complexity: 	O(n)
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
*******************************************************************************/
size_t FsaCountFree(const fsa_t *m_pool);


#endif    /*__FSA_H__*/


