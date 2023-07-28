/***************************************************************************
Author: Daniel Shabso
Reviewer: 
Date of Creation: 13.07.23
Last date of update:   
version: 1
****************************************************************************/
#include <assert.h>     /* assert                   */
#include <stdio.h>      /* printf                   */
#include <stdlib.h>     /* malloc, free, size_t     */
#include <time.h>       /* time                     */
#include <string.h>     /* memcpy                   */
#include "sort_funcs.h"

#define SUCCESS 1
#define FAILURE 0

/***************************************************************************/
typedef void (*sort_func)(int arr[], int size);
typedef int (*sort_func_radix)(unsigned int arr[], size_t size);

/***************************************************************************/
void BubbleSort(int arr[], int size);
void SelectionSort(int arr[], int size);
void InsertionSort(int arr[], int size);
void CountingSort(int arr[], int size);
int RadixSort(unsigned int arr[], size_t size);

static void Create_Array(int *arr, const int SIZE, const int MIN_VAL, const int MAX_VAL);
int Is_Array_Sorted(int *arr, const int size);
void Time_Measure(sort_func sortF, int arr[], int size);
void Time_MeasureForRadix(sort_func_radix sortF, unsigned int arr[], size_t size);

/***************************************************************************/
int main(int argc, char *argv[])
{
    int *arr = NULL;
    int *arr_to_work = NULL;
    int ARR_SIZE = 0;

    if (argc < 2)
    {
        printf("Usage: ./run ARR_SIZE\n");
        printf("for example: ./run 1000\n");
        exit(0);
    }

    ARR_SIZE = atoi(argv[1]);
    printf("arr size provided: %d\n", ARR_SIZE);
    if (ARR_SIZE <= 0)
    {
        printf("please provide an arr size bigger than 0\n");
        exit(0);
    }

    arr = (int *)malloc(ARR_SIZE * sizeof(int));
    arr_to_work = (int *)malloc(ARR_SIZE * sizeof(int));

    assert(arr);
    assert(arr_to_work);

    Create_Array(arr, ARR_SIZE, 0, 100000);
    /* print_array(arr, ARR_SIZE); */

    printf("BubbleSort:     ");
    memcpy(arr_to_work, arr, ARR_SIZE * sizeof(int));
    Time_Measure(BubbleSort, arr_to_work, ARR_SIZE);
    if (Is_Array_Sorted(arr_to_work, ARR_SIZE) == FAILURE)
    {
        printf("BubbleSort not working!!!\n");
    }

    printf("SelectionSort:   ");
    memcpy(arr_to_work, arr, ARR_SIZE * sizeof(int));
    Time_Measure(SelectionSort, arr_to_work, ARR_SIZE);
    if (Is_Array_Sorted(arr_to_work, ARR_SIZE) == FAILURE)
    {
        printf("SelectionSort not working!!!\n");
    }

    printf("InsertionSort:  ");
    memcpy(arr_to_work, arr, ARR_SIZE * sizeof(int));
    Time_Measure(InsertionSort, arr_to_work, ARR_SIZE);
    if (Is_Array_Sorted(arr_to_work, ARR_SIZE) == FAILURE)
    {
        printf("InsertionSort not working!!!\n");
    }

    printf("RadixSort:  ");
    memcpy(arr_to_work, arr, ARR_SIZE * sizeof(int));
    Time_MeasureForRadix(RadixSort, (unsigned int *)arr_to_work, ARR_SIZE);
    if (Is_Array_Sorted((int *)arr_to_work, ARR_SIZE) == FAILURE)
    {
        printf("RadixSort not working!!!\n");
    }
       
    free(arr);
    free(arr_to_work);

    return 0;
}

/***************************************************************************/
static void Create_Array(int *arr, const int SIZE, const int MIN_VAL, const int MAX_VAL)
{
    const int MODULU = MAX_VAL - MIN_VAL + 1;
    int i = 0;
    assert(MIN_VAL < MAX_VAL);
    srand(time(NULL));

    for (i = 0; i < SIZE; ++i)
    {
        arr[i] = rand() % MODULU;
    }
}

/* return 1 if sorted
 *        0 if not sorted
 */
int Is_Array_Sorted(int *arr, const int size)
{
    int i = 0;
    for (i = 0; i < size - 1; ++i)
    {
        if (arr[i] > arr[i + 1])
            return FAILURE;
    }
    return SUCCESS;
}

void Time_Measure(sort_func sortF, int arr[], int size)
{
	clock_t t = clock();
	
	sortF(arr, size);  /* Calling the function we want to measure */
		
    printf("elapsed time: %f milliseconds\n", (double)(clock() - t) * 1000.0 / CLOCKS_PER_SEC);
}

void Time_MeasureForRadix(sort_func_radix sortF, unsigned int arr[], size_t size)
{
	clock_t t = clock();
	
	sortF(arr, size);  /* Calling the function we want to measure */
		
	printf("elapsed time: %ld milliseconds\n", clock() - t);
}




