/*******************************************************************************
****** Author: HRD31
****** Date: 09.07.2023
****** Version: 1
****** Description: Variable size allocator DS header.
*******************************************************************************/
#include <stddef.h>  /* size_t */
#include <assert.h>

#define WORD_SIZE 8

#define FREE_BLOCK 	0
#define OCCUPUIED_BLOCK 1
occupuied
#define IS_FREE(a)  ((1) == (a & 1) ? (OCCUPUIED_BLOCK) : (FREE_BLOCK))


typedef struct vsa vsa_t;

/*in .c*/
struct vsa 
{
    size_t memory_size;
};

typedef struct block_header block_header_t;

struct block_header
{
	size_t size;
#ifndef NDEBUG
    long magic_number;
#endif
};
/*********************	Help Functions		***********************************/
size_t BlockSizeRoundUpToWordSize(size_t block_size)
{
	/* Deal with block_size < WORD_SIZE */
	block_size = WORD_SIZE > block_size ? WORD_SIZE : block_size;
	
	/* Deal with block_size not being a multiple of WORD_SIZE */
	block_size = 0 < (block_size % WORD_SIZE) ? 
			(block_size + WORD_SIZE - (block_size % WORD_SIZE)) : block_size;
	
	return block_size;
}

/*	Calculate Memory Size Suggest*/
size_t FsaSuggestSize(size_t memory_size)
{	
	memory_size = BlockSizeRoundUpToWordSize(memory_size);

	return (sizeof(vsa_t) + memory_size + WORD_SIZE);
}
/*******************************************************************************
Description:     	Initializes Memory management.  
Return value:    	Pointer to memory pool in case of success, otherwise NULL. 
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'memory' is invalid pointer.
                    pool_size must be larger than sizeof(word)*2.
*******************************************************************************/
vsa_t *VsaInit(void *memory, size_t memory_size)
{
	vsa_t *vsa = (vsa_t *)memory;
	char *ptr_to_first_block = NULL;
	
	assert(memory);
	vsa->memory_size = FsaSuggestSize(memory_size);	
	
	ptr_to_first_block = (char *)memory + sizeof(vsa_t);
	*(size_t *)ptr_to_first_block = vsa->memory_size - sizeof(vsa_t);
	
	return vsa;
}

/*******************************************************************************
Description:     	Allocates 'size' bytes.
Return value:    	Pointer to allocated memory in case of success, 
					otherwise NULL.
Time Complexity: 	O(n).
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
					If requested size is larger than largest chunk return NULL. 
*******************************************************************************/
void *VsaAlloc(vsa_t *m_pool, size_t size)
{
    char *memory_ptr = NULL;
    size_t block_size = 0;
    
    assert(m_pool);
    memory_ptr = (char *)m_pool + sizeof(vsa_t);    
    
    /* Move to the first free memory block if not at the end of memory */
    while (OCCUPUIED_BLOCK == IS_FREE(*(size_t *)memory_ptr) && 
           (size_t)memory_ptr < (size_t)m_pool + m_pool->memory_size)
    {
        block_size = *(size_t *)memory_ptr;
        memory_ptr += block_size;
    }
    
    /* Check if the requested size is larger than the largest chunk available */
    if (size > block_size - sizeof(size_t))
    {
        return NULL;
    }
    
    /* Mark the block as taken */
    *(size_t *)memory_ptr |= OCCUPUIED_BLOCK;
    
    /* Return a pointer to the allocated memory */
    return memory_ptr + sizeof(size_t);
}
/*******************************************************************************
Description:     	Free chunk of memory pointed by "m_chunk".
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'm_chunk' is invalid pointer.
*******************************************************************************/
void VsaFree(void *m_chunk)
{
    char *memory_ptr = (char *)m_chunk - sizeof(size_t);
    
    /* Mark the block as free */
    *(size_t *)memory_ptr &= ~OCCUPUIED_BLOCK;
}

/*******************************************************************************
Description:       Return size in bytes of the largest free chunk.  
Time Complexity:   O(n)
Notes:             Undefined behavior if 'm_pool' is invalid pointer.
*******************************************************************************/


size_t VsaLargestChunk(const vsa_t *m_pool)
{
    size_t free_cur = 0;
    size_t free_largest = 0;
    char *memory_ptr = NULL;A

    assert(m_pool);
    memory_ptr = (char *)m_pool + sizeof(vsa_t);

    /* Move to the first free memory block if not at the end of memory */
    while (OCCUPUIED_BLOCK == IS_FREE(*(size_t *)memory_ptr) && memory_ptr < (char *)m_pool + m_pool->memory_size)
    {
        memory_ptr += *(size_t *)memory_ptr;
    }

    /* Update free_cur to the size of the first block */
    free_cur = *(size_t *)memory_ptr;

    /* Move to the next free memory block until the end of memory */
    while (FREE_BLOCK == IS_FREE(*(size_t *)memory_ptr) && memory_ptr < (char *)m_pool + m_pool->memory_size)
    {
        free_cur += *(size_t *)memory_ptr;
        memory_ptr += *(size_t *)memory_ptr;
    }

    /* Update largest free chunk */
    free_largest = free_cur > free_largest ? free_cur : free_largest;

    return free_largest;
}

char *MoveToFirsFreeBlock()

