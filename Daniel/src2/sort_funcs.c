/*******************************************************************************
****** Author: HRD31
****** Date: 09.07.2023
****** Version: 1
****** Description: Variable size allocator DS header.
*******************************************************************************/
#include <stdlib.h>  /* malloc, free, size_t     */
#include <math.h>    /* pow   					 */
#include <assert.h>  /* assert                   */

#define YES 1
#define NO  0

#define SUCCESS 0
#define FAILURE 1

#define LUT_SIZE 256
#define BITS_PER_INT 32
#define MASK 0x000000FF

/*********************	Help Functions		***********************************/
static void SwapInt(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

static int IsBigger(int *a, int *b)
{
	if (*a > *b)
	{
		return YES;
	}
	
	return NO;
}

static int FindMin(int arr[], int size)
{
	int i = 0, min = arr[0];
	
	for (i = 0; i < (size -1); i++)
	{
		if(IsBigger(&min, &arr[i + 1]) == YES)
		{
			min = arr[i + 1];
		}
	}
	
	return min;
}

static int FindMax(int arr[], int size)
{
	int i = 0, max = arr[0];
	
	for (i = 0; i < (size -1); i++)
	{
		if(IsBigger(&max, &arr[i + 1]) == NO)
		{
			max = arr[i + 1];
		}
	}
	
	return max;
}
/*******************************************************************************
Description:     	Bubble Sort
Time Complexity: 	O(n^2)
*******************************************************************************/
void BubbleSort(int arr[], int size)
{
	int i = 0, j = 0;
	
	for (i = 0; i < (size -1); i++)
	{
		for (j = 0; j < (size - 1 - i); j++)
		{
			if(arr[j] > arr[j+1])
			{
				SwapInt(&arr[j], &arr[j+1]);
			}
		}
	}
	
}

/*******************************************************************************
Description:     	Selection Sort
Time Complexity: 	O(n^2)
*******************************************************************************/
void SelectionSort(int arr[], int size)
{
	int i = 0, j = 0, index = 0;
	
	for (i = 0; i < (size); i++)
	{
		index = i;
		for (j = i + 1; j < (size); j++)
		{
			index = ( arr[j] < arr[index] ) ? j : index;
		}
		SwapInt(&arr[i], &arr[index]);
	}
	
}
/*******************************************************************************
Description:     	Insertion Sort
Time Complexity: 	O(n^2)
*******************************************************************************/
void InsertionSort(int arr[], int size)
{
	int i = 1;
	
	for (i = 1; i < size; i++) 
	{
		int sort_num = arr[i];
		int j = i - 1;

		while (sort_num < arr[j] && j >= 0) 
			{
				SwapInt(&arr[j + 1], &arr[j]);
			 	 --j;
			}
	}
}

/*******************************************************************************
Description:     	Counting Sort
Time Complexity: 	O(n^2)
*******************************************************************************/
int *CountingSortt(int arr[], int size)
{
	int min = FindMin(arr, size);
	int max = FindMax(arr, size);
	int rang = 	1 + max - min;
	int i = 0, tmp1 = 0, tmp2 = 0;
	
	int *lut = (int *)calloc(rang*sizeof(int));
	if(NULL == lut)
	{
		return NULL;
	}
	
	int *result = 	(int *)calloc(size*sizeof(int));
	if(NULL == result)
	{
		return NULL;
	}
	
	/*initiolaize LOT*/
	for(i = 0; i < size; i++)
	{
		lut[arr[i]-min] += 1;
	}

	/*initiolaize offset*/
	tmp1 = lut[0];
	lut[0] = 0;	
	
	for(i = 1; i < rang; i++)
	{
		tmp2 = lut[i];
		lut[i] = tmp1;
		tmp1 += tmp2;
	}

	for(i = 0; i < size; i++)
	{		
		result[lut[arr[i] - min]] = arr[i];
		lut[arr[i] - min] += 1;
	}
	
	for(i = 0; i < size; i++)
	{
		arr[i] = result[i];
	}	
	
	free(lut);
	lut = NULL;
	free(result);
	lut = NULL;
	
	return arr;
}

/*******************************************************************************
Description:     	Radix Sort
Time Complexity: 	O(n)
*******************************************************************************/
static void RadixSortImp(int *arr, size_t size, int *helper_arr, unsigned int bits)
{
	int mask_bits = 1, i = 0, hist_size = pow(2, bits), k = 0;
	int curr = 0, sum_prev = 0, num_of_iterations = BITS_PER_INT/bits;
	int *hist = NULL, *temp = NULL;
	size_t j = 0;
	
	assert(NULL != arr);
	assert(NULL != helper_arr);
	
	hist = (int *)calloc(hist_size, sizeof(int));
	if(NULL == hist)
	{
		return FAILURE;
	}
		
	/* Create mask for first 'bits' */
	mask_bits <<= bits;
	mask_bits -= 1;
	
	for(k = 0; k < num_of_iterations; k++)
	{
		/* Initialize hist */
		for(i = 0; i < hist_size; i++)
		{
			hist[i] = 0;
		}
		
		sum_prev = 0;
		
		for(j = 0; j < size; j++)
		{
			++hist[(arr[j] >> (k * bits)) & mask_bits];
		}
		/* Convert hist to offset table */
		for(i = 0; i < hist_size; i++)
		{
			curr = hist[i];
			hist[i] = sum_prev;
			sum_prev += curr;
		}
		
		for(j = 0; j < size; j++)
		{
			helper_arr[hist[(arr[j] >> (k * bits)) & mask_bits]] = arr[j];
			++(hist[(arr[j] >> (k * bits)) & mask_bits]);
		}
		
		temp = arr;
		arr = helper_arr;
		helper_arr = temp;
	}
	
	free(hist);
	return SUCCESS;
}

int RadixSort(unsigned int arr[], size_t size)
{
	int *helper_arr = NULL;
	
	assert(NULL != arr);
	
	helper_arr = (int *)calloc(size, sizeof(int));
	
	RadixSortImp((int *)arr, size, helper_arr, hist, 4);
	
	free(helper_arr);
	
	return 0;
}


































