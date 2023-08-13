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
#include <time.h>

#define LUT_SIZE N*N
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
void PrintIntArray(int arr[], int size)
{
	int i = 0;
	
    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

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
 
static void printSolution(size_t *bit_track, int current) 
{
	int i = 0, j = 0, position = 0;
	
    for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++) 
        {
			position = N * i + j;
			if (current == position)
			{
				printf("$ ");				
			}
			else
			{
				printf("%d ", BitArrGetVal(*bit_track, position));
			}        
        }
        printf("\n");
    }
	printf("\n");
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


/* return index which we can move to from index lut_index, 
 * there can be up to 8 different possibilites and the function 
 * retruns randomly one of them
 */
static int MoveToRandomPossibleIndex(const int lut_index, lut_t lut_table[])
{
	int k, lut_index_new;
	k = rand() % 8;
	while (NON_VAL == lut_table[lut_index].arr[k]) 
	{
		k = rand() % 8;
	}
	
	lut_index_new = lut_table[lut_index].arr[k];
	return lut_index_new;
}

/* return index which we can move to from index lut_index, 
 * we can only move if it's free place which we didn't already been
 * retruns -1 if there is no free place
 */
static int tryMoveToFreeIndex(const int lut_index, size_t *bit_track, lut_t lut_table[])
{
	int k, lut_index_new;
	for (k = 0; k < 8; k++) 
	{
		/* first check if the move valid */
		if (NON_VAL != lut_table[lut_index].arr[k])
		{
			lut_index_new = lut_table[lut_index].arr[k];
			
			if (free_place == BitArrGetVal(*bit_track, lut_index_new))
			{
				/* found valid and free place to move */
				return lut_index_new;
			}
		}
	}
	
	/* didn't find free place to jump to */
	return -1;
}

#define MAX_RECURSION_DEPTH 1000
int solveKTUtil(int lut_index, size_t *bit_track, lut_t lut_table[])
{
	int k = 0, found_free = 0, idx_res = 0;
	int lut_index_new = 0; 
	
	static size_t recursion_counter = 0; /* counter to check how many times the recursion occurs */
	++recursion_counter;
	
	if (ALL_ON == *bit_track) 
	{
		return true;
	}
	
	lut_index_new = tryMoveToFreeIndex(lut_index, bit_track, lut_table); 
	if (-1 != lut_index_new) {
		*bit_track = BitArrSetOn(*bit_track, lut_index_new);
		found_free = 1;
	}
	else 
	{
		lut_index_new = MoveToRandomPossibleIndex(lut_index, lut_table); 
	}
	
	printf("\ncnt = %ld, found free to jump = %d\n", recursion_counter, found_free);
	printSolution(bit_track,  lut_index_new);
	
	if (0 == found_free && MAX_RECURSION_DEPTH < recursion_counter)
	{
		/*did not found a valid next move*/
		printf("\n%08X\n", *bit_track);
		return false;
	}
	
	return solveKTUtil(lut_index_new, bit_track,  lut_table);
	/*
	if (solveKTUtil(lut_index_new, bit_track,  lut_table)) 
	{
		return true;
	}
	
	return false;    */
}



int solveKT(int x, int y, lut_t lut_table[]) 
{
	size_t bit_track = 0;
	int lut_index = x * N + y;
	int result[LUT_SIZE] = {0};
	bit_track = BitArrSetOn(bit_track, lut_index);
    /*If a solution exists, print the solution*/
    if (solveKTUtil(lut_index, &bit_track, lut_table)) 
    {
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
    srand(time(NULL));


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
        printf("\n");
        printf("%2d: ", i);
        for (j = 0; j < N; j++)
        {
            printf("%2d ", lut_table[i].arr[j]);
        }
    }
#endif
	i = 0;
	j = 0;
	
    solveKT(i, j, lut_table);
    	
    return 0;
}

















