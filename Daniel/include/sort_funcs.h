/*******************************************************************************
****** Author: HRD31
****** Date: 09.07.2023
****** Version: 1
****** Description: Variable size allocator DS header.
*******************************************************************************/
#ifndef __SORT_FUNCS__
#define __SORT_FUNCS__

#include <stddef.h>  /* size_t */

/*******************************************************************************
*******************************************************************************/
void BubbleSort(int arr[], int size);


/******************************************************************************* 
*******************************************************************************/
void SelectionSort(int arr[], int size);
    

/*******************************************************************************
*******************************************************************************/
void InsertionSort(int arr[], int size);


/*******************************************************************************
*******************************************************************************/
void CountingSortt(int arr[], int size);

/*******************************************************************************
*******************************************************************************/
int radixSort(unsigned int arr[], size_t size);



#endif    /*__VSA_H__*/

