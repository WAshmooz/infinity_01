/*******************************************************************************
****** Author: HRD31
****** Date: 11.06.2023
****** Version: 1
*******************************************************************************/
/*******************************************************************************
****** Author: HRD31
****** Date: 14.06.2023
****** Version: 1
*******************************************************************************/
#ifndef __CBUFFER_H__
#define __CBUFFER_H__

#include <unistd.h>	 /* ssize_t, size_t */

typedef struct cbuffer cbuffer_t;

/*******************************************************************************
Description:     	Creates an empty circular buffer of 'capacity' size in bytes.
Return value:    	Pointer to cbuffer in case of success, otherwise NULL.
Time Complexity: 	Determined by system call complexity.
Note:            	Should call "CBufferDestroy()" at end of use.
*******************************************************************************/
cbuffer_t *CBufferCreate(size_t capacity);


/*******************************************************************************
Description:     	Deletes a cbuffer pointed by 'cbuffer' from memory.
Time Complexity: 	Determined by system call complexity. 
Notes:           	Undefined behaviour if 'cbuffer' is invalid pointer.
*******************************************************************************/
void CBufferDestroy(cbuffer_t *cbuffer);


/*******************************************************************************
Description:     	Returns number of free bytes.      
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'cbuffer' is invalid pointer.
*******************************************************************************/
size_t CBufferFreeSpace(const cbuffer_t *cbuffer);


/*******************************************************************************
Description:     	Returns number of occupied bytes.         
Time Complexity: 	O(1)
Notes:              Undefined behaviour if 'cbuffer' is invalid pointer.
*******************************************************************************/
size_t CBufferBufSize(const cbuffer_t *cbuffer);


/*******************************************************************************
Description:    	Attempts to read up to 'n_bytes' from 'cbuffer' into the 
					buffer starting at 'dest'.
Return value: 		On success, the number of bytes read is returned
					If "cbuffer" is empty, -1 is returned
Time Complexity:   	O(n).
Notes:             	Undefined behaviour if 'cbuffer' or 'dest' is invalid
                    pointer.         
*******************************************************************************/
ssize_t CBufferRead(cbuffer_t *cbuffer, void *dest, size_t n_bytes);


/*******************************************************************************
Description:    	Attempts to write up to 'n_bytes' from buffer starting at 
					'src' into 'cbuffer' starting from its free space.
Return value: 		On success, the number of bytes written is returned
					If "cbuffer" is full, -1 is returned
Time Complexity:   	O(n)
Notes:             	Undefined behaviour if 'cbuffer' or 'src' is invalid
                    pointer.          
*******************************************************************************/
ssize_t CBufferWrite(cbuffer_t *cbuffer, const void *src, size_t n_bytes);


/*******************************************************************************
Description: 		Checks if "cbuffer" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'cbuffer' is invalid pointer.
*******************************************************************************/
int CBufferIsEmpty(const cbuffer_t *cbuffer);

#endif    /*__CBUFFER_H__*/






























