/*******************************************************************************
****** Author: HRD31
****** Date: 04.06.2023
****** Version: 1
*******************************************************************************/
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>  /* size_t */

typedef struct vector vector_t;

/*******************************************************************************
description:     create a vector of "capacity" elements of elem_size
arguments:       capacity - number of elements.
                 elem_size in bytes.
return value:    pointer to vector in case of success, otherwise NULL.
Time Complexity: determined by system call complexity. 
Note:            should call "VectorDestroy()" at end of use.
*******************************************************************************/
vector_t *VectorCreate(size_t capacity, size_t elem_size);


/*******************************************************************************
description:     delete a vector pointed by "vector" from memory.
Time Complexity: determined by system call complexity. 
Notes:           if vector is NULL, no operation is performed.
*******************************************************************************/
void VectorDestroy(vector_t *vector);


/*******************************************************************************
description:     add "element" to the end of vector pointed by "vector".
return value:    0 for success, 1 for fail            
Time Complexity: determined by system call complexity, best case O(1).
                 undefined behaviour if element doesn't match elem_size
*******************************************************************************/
int VectorPushBack(vector_t *vector, void *element);


/*******************************************************************************
description:     deletes the last element in vector pointed by "vector".
Time Complexity: O(1)
Notes:           undefined behaviour in case of empty vector.
*******************************************************************************/
void VectorPopBack(vector_t *vector);


/*******************************************************************************
description:     return the "index" element from "vector" 
Time Complexity: O(1)
Notes:           undefined behaviour in case of out of range index.
*******************************************************************************/
void *VectorGetAccess(const vector_t *vector, size_t index);


/*******************************************************************************
description:     return number of occupied elements in "vector"
Time Complexity: O(1)
*******************************************************************************/
size_t VectorSize(const vector_t *vector);


/*******************************************************************************
description:     returns total number of element in "vector".
Time complexity: O(1)
*******************************************************************************/
size_t VectorCapacity(const vector_t *vector);


/*******************************************************************************
description:     set the capacity of "vector" to "new_capacity".
return value:    0 for success, 1 for fail  
arguments:       new_capacity in number of elements.
Time complexity: determined by system call complexity. 
Note:            new_capacity must be larger than vector's current capacity,
                 otherwise undefined behaviour
*******************************************************************************/
int VectorReserve(vector_t *vector, size_t new_capacity);


/*******************************************************************************
description:     set the capacity of "vector" to vector size.
return value:    0 for success, 1 for fail  
Time complexity: determined by system call complexity. 
*******************************************************************************/
int VectorShrinkToFit(vector_t *vector);


#endif    /*__VECTOR_H__*/
