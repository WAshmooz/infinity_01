    /*******************************************************************************
    ****** Author: HRD31
    ****** Date: 01.08.2023
    ****** Version: 1
    *******************************************************************************/
    #ifndef __HEAP_H__
    #define __HEAP_H__

    #include <stddef.h>  		/* size_t       */
    #include "aux_funcs.h"      /* is_bis_before_t, is_match_func_tefore_t	*/

    typedef struct heap heap_t;
    
    
    /*******************************************************************************
    Description:     	Creates an empty heap, according to 'is_before_func'.
    Return value:    	Pointer to heap in case of success, otherwise NULL.
    Time Complexity: 	O(1). 
    Note:            	Should call 'HeapDestroy()' at end ofuse.
    *******************************************************************************/
    heap_t *HeapCreate(is_before_t is_before_func);


    /*******************************************************************************
    Description:     	Deletes a heap pointed by 'heap' from memory.
    Time Complexity: 	O(1) 
    Notes:           	Undefined behaviour if 'heap' is invalid pointer.
    *******************************************************************************/
    void HeapDestroy(heap_t *heap);


    /*******************************************************************************
    Description:     	Add 'data' to heap pointed by 'heap'.
    Return value:    	0 for success, 1 for fail.         
    Time Complexity: 	Amortized O(log(n)).
    Notes:              Undefined behaviour if 'heap' is invalid pointer.
    *******************************************************************************/
    int HeapPush(heap_t *heap, void *data);


    /*******************************************************************************
    Description:       Deletes the first element in 'heap'.
    Time Complexity:   O(log(n))
    Notes:             Undefined behaviour if 'heap' is invalid pointer.          
    *******************************************************************************/
    void HeapPop(heap_t *heap);


    /*******************************************************************************
    Description:   		Returns data of the first element in 'heap'. 
    Time complexity:  	O(1)
    Note: 				Undefined behaviour if 'heap' is invalid pointer.
    *******************************************************************************/
    void *HeapPeek(const heap_t *heap);


    /*******************************************************************************
    Description: 		Checks if 'heap' is empty.
    Return value:   	1 for true, 0 for false.
    Time complexity:  	O(1)
    Note: 				Undefined behaviour if 'heap' is invalid pointer.
    *******************************************************************************/
    int HeapIsEmpty(const heap_t *heap);


    /*******************************************************************************
    Description:     	Returns number of elements in 'heap'
    Time Complexity: 	O(1)
    Notes:			 	Undefined behaviour if 'heap' is invalid pointer
    *******************************************************************************/
    size_t HeapSize(const heap_t *heap);


    /*******************************************************************************
    Description:  	  	Erase the element with 'data' found by 'is_before_func' from
                        a heap pointed by 'heap'.
    Return value 		0 for success otherwise 1.
    Time complexity:  	O(n)
    Notes:            	Undefined behaviour if 'heap' is invalid pointer.
    *******************************************************************************/
    int HeapRemove(heap_t *heap, is_match_func_t is_match, void *param);


    #endif    /*__HEAP_H__*/
