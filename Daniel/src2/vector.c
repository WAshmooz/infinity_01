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
#include "../include/vector.h"

#define GROWTH_FACTOR 2
/******************************************************************************/
struct vector
{
	size_t size;		
	size_t capacity;	
	size_t elem_size;	
	void *data;			
};


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
	
	free(vector->data);
	vector->data = NULL;
	free(vector);
}

/******************************************************************************/
int VectorPushBack(vector_t *vector, void *element)
{
	char *data_char_ptr = (char *)vector->data;

	assert(NULL != vector);
	assert(NULL != element);
	
	if (vector->size ==  vector->capacity)
	{
		vector->data = (void *)realloc( vector->data, GROWTH_FACTOR * (vector->elem_size * vector->capacity) );
		vector->capacity *= GROWTH_FACTOR;
	}															
	
	if (vector->data == NULL)
	{
		vector->data = (void *)data_char_ptr;
		return 1;
	}
	
	data_char_ptr = (char*)vector->data;
	data_char_ptr += (vector->size) * (vector->elem_size);
	memcpy(data_char_ptr, element, vector->elem_size);
	++(vector->size);
	
   return (vector->data != NULL);
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








