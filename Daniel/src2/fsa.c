/***************************************************************************
Author: Daniel Shabso
Reviewer: Maron
Date of Creation: 6.7.23
Last date of update:   
version: 1
****************************************************************************/
#include <stdlib.h>  /* malloc, free, size_t */
#include <assert.h>  /* assert */
#include <stdio.h>

#define WORD_SIZE 8

#ifndef NDEBUG
#include <stdio.h>
#endif

struct fsa
{
	void *first_free_block;
};


typedef struct fsa fsa_t;

/*******************************************************************************
Description:     	Calculate block_size feet to WORD SIZE (Help function)
Return value:    	size of block_size.
Time Complexity: 	O(1). 
*******************************************************************************/
size_t BlockSizeRoundUpToWordSize(size_t block_size)
{
	/* Deal with block_size < WORD_SIZE */
	block_size = WORD_SIZE > block_size ? WORD_SIZE : block_size;
	
	/* Deal with block_size not being a multiple of WORD_SIZE */
	block_size = 0 < (block_size % WORD_SIZE) ? 
			(block_size + WORD_SIZE - (block_size % WORD_SIZE)) : block_size;
	
	return block_size;
}
/*******************************************************************************
Description:     	Calculate Memory Pool Size 
Return value:    	size of memory pool.
Time Complexity: 	O(1).
notes:              If 'block_size' < sizeof(size_t) then 
                    block_size = sizeof(size_t).
*******************************************************************************/
size_t FsaSuggestSize(size_t num_of_block, size_t block_size)
{	
	block_size = BlockSizeRoundUpToWordSize(block_size);

	return (num_of_block * block_size + sizeof(fsa_t));
}

/*******************************************************************************
Description:     	Initializes Memory managment.  
Return value:    	Pointer to memory pool in case of success, otherwise NULL. 
Time Complexity: 	O(n)
notes:              Should call 'FsaSuggestSize()' to calculate 'pool_size'.
*******************************************************************************/
fsa_t *FsaInit(void *memory, size_t pool_size, size_t block_size)
{
	fsa_t *fsa = (fsa_t *)memory;
	char *ptr = NULL;
	char *next_block = NULL;
	size_t num_of_blocks = 0, i = 0;
	
	assert(memory);
	block_size = BlockSizeRoundUpToWordSize(block_size);
	
	/*fsa: point to the start of the memory */
    fsa->first_free_block = (char *)fsa + sizeof(fsa_t);
    ptr = (char *)fsa->first_free_block;
    num_of_blocks = (pool_size - sizeof(fsa)) / block_size;
	
    for(i = 0; i < num_of_blocks - 1; ++i)
    {
        next_block = ptr + block_size;  
        *(size_t *)ptr = (size_t)next_block; /* write next address free */
        ptr = next_block;					 /* move to next address free */
    }

    *(size_t *)ptr = 0; /* write NULL in the last address */

    return fsa;
}


/*******************************************************************************
Description:     	Return pointer to memory Block
Return value:    	Pointer to memory block in case of success, otherwise NULL        
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
*******************************************************************************/
void *FsaAlloc(fsa_t *m_pool)
{
    char *curr_block = NULL;

    assert(m_pool);

    curr_block = (char *)m_pool->first_free_block;

    if (NULL == curr_block)
    {
        return NULL;
    }

    m_pool->first_free_block = (void *)(*(size_t *)curr_block);

    return curr_block;
} 

/*******************************************************************************
Description:     	Free block of memory pointed by "m_block".
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
                    Undefined behaviour if 'm_block' is invalid pointer.
                    Undefined behaviour if 'm_block' has been freed already.
*******************************************************************************/
void FsaFree(fsa_t *m_pool, void *m_block)
{
    assert(m_pool);
    assert(m_block);

    *(size_t *)m_block = (size_t)m_pool->first_free_block;
    m_pool->first_free_block = m_block;
}

/*******************************************************************************
Description:     	Return Numbers of free blocks.  
Time Complexity: 	O(n)
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
*******************************************************************************/
size_t FsaCountFree(const fsa_t *m_pool)
{
    size_t *ptr = (size_t *)m_pool->first_free_block;
    size_t count = 0;

    while (ptr)
    {
        ptr = (size_t *)*ptr;
        ++count;
    }

    return count;
}




