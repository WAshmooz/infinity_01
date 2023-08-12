/*******************************************************************************
****** 	Author:			HRD31
******	Author: 		Daniel Shabso
******	Reviewer: 		xxx 
******	Date: 			6.08.2023
****** 	Version: 		1
******	Description: 	DHCP server.
*******************************************************************************/

/**********************		Headers		***************************************/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define LUT_SIZE 64
#define N 		  8
#define NON_VAL  -1

#define ONLY_MSB_ON		0X8000000000000000
#define MASK_ONE		(size_t)1
#define ALL_ON 			0Xffffffffffffffff

typedef struct
{
    int arr[N];
    int cur;
} lut_t;

enum { false = 0, true = 1 };
enum { free_place = 0, not_free_place = 1 };
/*******************************************************************************
                            Helper Functions
*******************************************************************************/ 
/*sort functions*/
size_t BitArrSetOn(size_t bit_arr, size_t idx)
{
	assert(idx < 64);
	return bit_arr | (MASK_ONE << idx);
}

int BitArrGetVal(size_t bit_arr, size_t idx)
{
	assert(idx < 64);
	return 1 & (bit_arr >> idx);
}

static void SwapInt(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

static int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1), j= 0;
 
    for (j = low; j <= high - 1; j++) 
    {
        if (arr[j] < pivot) 
        {

            i++;
            SwapInt(&arr[i], &arr[j]);
        }
    }
    SwapInt(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
static void quickSort(int arr[], int low, int high)
{
    if (low < high) 
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


static void SetLUT(lut_t lut_table[])
{
    int i = 0, j = 0, k = 0;
    int x_move[N] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int y_move[N] = { 1, 2, 2, 1, -1, -2, -2, -1 };   
	

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
        	for (k = 0; k < N; k++)
        	{
				/* Calculate the index in the LUT using i and j */
				int lut_index = i * N + j;
			 
				/* Calculate the new x and y positions */
				int new_x = i + x_move[k];
				int new_y = j + y_move[k];
				
				/* Check if in range */
				if (new_x >= 0 && new_x < N && new_y >= 0 && new_y < N)
				{
				    /* Calculate the new index in the LUT */
				    int new_lut_index = new_x * N + new_y;
				    
				    /* Assign the value to the LUT entry */
				    lut_table[lut_index].arr[k] = new_lut_index;
				}
				else
				{
				    /* Assign -1 if not in range*/
				    lut_table[lut_index].arr[k] = NON_VAL;
				}
            }
        }
    }
}


int solveKTUtil(int x, int y, size_t *bit_track, lut_t lut_table[])
 {
 	int k = 0;
 	int lut_index = x * N + y;
 	int lut_index_new = 0; 	
    if (ALL_ON == *bit_track) 
    {
        return true;
    }

    for (k = 0; k < 8; k++) 
    {
    /*if there is valid move that is not used yet go there*/
		if (NON_VAL != lut_table[lut_index].arr[k] 
			&& free_place == BitArrGetVal(*bit_track, lut_index))
		{
			*bit_track = BitArrSetOn(*bit_track, lut_index);
			lut_index_new = lut_table[lut_index].arr[k];
		}
	}	
	
	if (lut_index_new == lut_index)
	{
		/*did not found a valid next move*/
        return false;		
	}

    if (solveKTUtil(x, y, bit_track,  lut_table)) 
    {
        return true;
    }
    
    return false;    
}



int solveKT(int x, int y, lut_t lut_table[]) 
{
    /*If a solution exists, print the solution*/
    if (solveKTUtil(x, y, 0, lut_table)) 
    {
        /*printSolution(lut_table);*/
        return true;
    } 
    
    else 
    {
        printf("No solution exists\n");
        return false;
    }
}






/*******************************************************************************
								MAIN	
*******************************************************************************/
int main()
{
    int i = 0, j = 0;
    lut_t lut_table[LUT_SIZE];


    for (i = 0; i < LUT_SIZE; i++)
    {
        for (j = 0; j < N; j++)
        {
            lut_table[i].arr[j] = 0;  
        }
    }

    SetLUT(lut_table);

#if 1	
    printf("\n********    TEST 2    *********\n");
    for (i = 0; i < LUT_SIZE; i++)
    {
    	int lut_index = i * N + j;
        printf("\n");
        for (j = 0; j < N; j++)
        {
            printf("%d ", lut_table[i].arr[j]);
            printf("%d ", lut_table[i].arr[j]);
        }
    }
#endif
	i = 0;
	j = 0;
	
    solveKT(i, j, lut_table);
    	
    return 0;
}

















