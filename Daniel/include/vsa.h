/*******************************************************************************
****** Author: HRD31
****** Date: 09.07.2023
****** Version: 1
****** Description: Variable size allocator DS header.
*******************************************************************************/
#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h>  /* size_t */

typedef struct vsa vsa_t;

/*******************************************************************************
Description:     	Initializes Memory management.  
Return value:    	Pointer to memory pool in case of success, otherwise NULL. 
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'memory' is invalid pointer.
                    pool_size must be larger than sizeof(word)*2.
*******************************************************************************/
vsa_t *VsaInit(void *memory, size_t memory_size);


/*******************************************************************************
Description:     	Allocates 'size' bytes.
Return value:    	Pointer to allocated memory in case of success, 
					otherwise NULL.
Time Complexity: 	O(n).
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
					If requested size is larger than largest chunk return NULL. 
*******************************************************************************/
void *VsaAlloc(vsa_t *m_pool, size_t size);
    

/*******************************************************************************
Description:     	Free chunk of memory pointed by "m_chunk".
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'm_chunk' is invalid pointer.
*******************************************************************************/
void VsaFree(void *m_chunk);


/*******************************************************************************
Description:     	Return size in bytes of largest free chunk.  
Time Complexity: 	O(n)
Notes:              Undefined behaviour if 'm_pool' is invalid pointer.
*******************************************************************************/
size_t VsaLargestChunk(const vsa_t *m_pool);


#endif    /*__VSA_H__*/


