/*******************************************************************************
****** Author: HRD31
******	Author: 	Daniel Shabso
******	Reviewer: 	Michael 
****** Date: 30.05.2023
****** Version: 1
*******************************************************************************/

/**********************		Headers		***************************************/

#include <stdio.h>  /* printf, size_t */
#include <stdlib.h> /* malloc         */
#include <assert.h> /* assert         */
#include <string.h> /* memcpy         */
#include "../include/stack.h"

/******************************************************************************/
struct stack
{
	size_t size;		/*	current num of elem in stack	*/
	size_t capacity;	/*	max num of elem in stack		*/
	size_t elem_size;	/*	size of each elem in stack		*/
	void *data;			/*	pointer to start of stack		*/
};

/******************************************************************************/
/*	Create function	*/
stack_t *StackCreate(size_t elem_num, size_t elem_size_)
{
	stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
	
	if(NULL == stack)
	{
		return NULL;
	}
	
	stack->data = (void *)malloc(elem_size_ * elem_num);
		
	if(NULL == stack->data)
	{
		printf("Memory allocation failed");
	}
	
	stack->capacity = elem_num;
	stack->size = 0;
	stack->elem_size = elem_size_;
	
	return stack;
}

/*	Destroy function	*/
void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	
	free(stack->data);
	stack->data = NULL;
	free(stack);
}

/*	Push function	*/
void StackPush(stack_t *stack, void *element)
{
	char *data_char_ptr = (char *)stack->data;
	
	assert(NULL != stack);
	assert(NULL != element);
	
	data_char_ptr += (stack->size) * (stack->elem_size);
	++(stack->size);
	memcpy(data_char_ptr, element, stack->elem_size);
}

/*	Pop function	*/
void StackPop(stack_t *stack)
{
	assert(NULL != stack);
	--(stack->size);
}

/*	Peek function	*/
void *StackPeek(const stack_t *stack)
{
	char *data_char_ptr = (char *)(stack->data);
	
	assert(NULL != stack);
	
	data_char_ptr += (stack->size - 1) * (stack->elem_size);
	
	return (void *)data_char_ptr;
}

/*	Size function	*/
size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);
	
	return stack->size;
}

/*	IsEmpty function	*/
int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (stack->size == 0);
}

/*	Capacity function	*/
size_t StackCapacity(const stack_t *stack)
{
	assert(NULL != stack);
	
	return stack->capacity;
}











