/*******************************************************************************
****** Author: HRD31
****** Date: 31.05.2023
****** Version: 1
*******************************************************************************/
#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h>  /* size_t */

typedef struct stack stack_t;


/*******************************************************************************
description:     create a stack of size "element_num * elem_size" of type stack_t. 
arguments:       elem_size in bytes 
return value:    pointer to stack in case of success, otherwise NULL
Time Complexity: determined by system call complexity. 
Note:            should call "StackDestroy()" at end of use.
*******************************************************************************/
stack_t *StackCreate(size_t elem_num, size_t elem_size);


/*******************************************************************************
description:     delete a stack pointed by "stack" from memory.
Time Complexity: determined by system call complexity. 
Notes:            if stack is NULL, no operation is performed.
*******************************************************************************/
void StackDestroy(stack_t *stack);


/*******************************************************************************
description:     add "element" to the end in stack pointed by "stack".
Time Complexity: O(1)
Notes:           undefined behaviour in case of full stack.
*******************************************************************************/
void StackPush(stack_t *stack, void *element);


/*******************************************************************************
description:     deletes the last element in stack pointed by "stack".
Time Complexity: O(1)
Notes:           undefined behaviour in case of empty stack.
*******************************************************************************/
void StackPop(stack_t *stack);


/*******************************************************************************
description:     return the last element in "stack"
Time Complexity: O(1)
Notes:           undefined behaviour in case of empty stack.
*******************************************************************************/
void *StackPeek(const stack_t *stack);


/*******************************************************************************
description:     return number of occupied elements in "stack"
Time Complexity: O(1)
*******************************************************************************/
size_t StackSize(const stack_t *stack);


/*******************************************************************************
description:     checks if "stack" is empty.
return value:    1 if true, 0 if false
time complexity: O(1)
*******************************************************************************/
int StackIsEmpty(const stack_t *stack);


/*******************************************************************************
description:     returns total number of element in stack.
Time complexity: O(1)
*******************************************************************************/
size_t StackCapacity(const stack_t *stack);


#endif    /*__STACK_H__*/
