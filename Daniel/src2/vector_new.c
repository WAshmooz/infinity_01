/*******************************************************************************
****** Author: HRD31
****** Date: 30.05.2023
****** Version: 1
*******************************************************************************/

/**********************		Headers		***************************************/

#include <stdio.h>  /* size_t 			*/
#include <stdlib.h> /* malloc ,realloc  */
#include <assert.h> /* assert        	*/
#include <string.h> /* memcpy         	*/
#include "vector.h"

#define GROWTH_FACTOR 2
/******************************************************************************/
struct vector
{
	size_t size;		
	size_t capacity;	
	size_t elem_size;	
	void *data;			
};

/*******************************************************************************
description:     resize function used by reserve and shrinktofit
return value:    success or failed in case of realloc fail.  
Time Complexity: system call complexity.
*******************************************************************************/
int Resize(vector_t *vector, size_t new_capacity)
{
	vector_t *tmp = realloc(vector->data, new_capacity);
	if(NULL == tmp)
	{
		return 0;
	}
	vector->data = tmp;
	return 1;	
}
/******************************************************************************/
vector_t *VectorCreate(size_t capacity, size_t elem_size)
{
	vector_t *vector = (vector_t *)malloc(sizeof(vector_t));
	
	if(NULL == vector)
	{
		return NULL;
	}
	
	vector->data = (void *)malloc(elem_size * capacity);
		
	if(NULL == vector->data)
	{
		printf("Memory allocation failed");
	}
	
	vector->size = 0;
	vector->capacity = capacity;
	vector->elem_size = elem_size;
	
	return vector;
}

/******************************************************************************/
void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);
	
	free(vector->data); vector->data = NULL;
	free(vector);	
}

/******************************************************************************/
#define SUCCESS 			1
#define FAILED 				0 
#define CAP_BYTES(vector) 	(vector->capacity * vector->elem_size)
#define TOP_BYTES(vector)	((vector->size)*(vector->elem_size))
#define GROWTH_FACTOR  2
#define VECTOR_SUCCESS 0
#define VECTOR_FAILURE 1

int VectorPushBack(vector_t *vector, void *element)
{
	void *dest = NULL;
	char *char_dest = NULL;
	
	assert(NULL != vector);
	assert(NULL != element);
	
	if(vector->size == vector->capacity)
	{
		if(FAILED == Resize(vector, CAP_BYTES(vector) * GROWTH_FACTOR))
		{
			return VECTOR_FAILURE;
		}
		vector->capacity *= GROWTH_FACTOR;
	}

	char_dest = (char*)vector->data;
	char_dest += TOP_BYTES(vector);
	dest = char_dest;
	memmove(dest, element, vector->elem_size);
	
	++(vector->size);
	
	return VECTOR_SUCCESS;
}


/******************************************************************************/
void VectorPopBack(vector_t *vector)
{
	assert(NULL != vector);
	--(vector->size);
}


/******************************************************************************/
void *VectorGetAccess(const vector_t *vector, size_t index)
{
	char *data_char_ptr = (char *)(vector->data);
	
	assert(NULL != vector);
	
	data_char_ptr += (index) * (vector->elem_size);
	
	return (void *)data_char_ptr;
}

/******************************************************************************/
size_t VectorSize(const vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->size;
}

/******************************************************************************/
size_t VectorCapacity(const vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->capacity;
}


/******************************************************************************/
int VectorReserve(vector_t *vector, size_t new_capacity)
{
	assert(NULL != vector);

	vector->data = (void *)realloc( vector->data, new_capacity * vector->elem_size);
	vector->capacity = new_capacity;
	
	return (vector->data != NULL);
}

/******************************************************************************/
int VectorShrinkToFit(vector_t *vector)
{
	assert(NULL != vector);

	vector->data = (void *)realloc( vector->data, (vector->size * vector->elem_size) );
	vector->capacity = (vector->size) ;
	
	return (vector->data != NULL);
}








