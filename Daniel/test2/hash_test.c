/******************************************************************************  
****	dlist_test.c
****	Author: 	Daniel Shabso
****	Reviewer: 	 
****	Date: 		31.07.23
****	Version: 	0
****    Description: Data structure that maps keys to values.
******************************************************************************/

/*************************	Headers	and Macro	*******************************/
#include <stdio.h> /* printf, size_t */
#include "hash.h"
	
#define NUM_OF_TESTS 8

#define SUCCESS 0
#define FAIL 	1
/******************************************************************************/

/***********************	Functions declarations		***********************/
static void TestCreateAndDestroy(void);
static void TestInsertRemove(void);

static int MatchInts(const void *, const void *);
static size_t HashFunc(const void *key);
/******************************************************************************/

/***************************	Main	**************************************/

int main(void)
{
	TestCreateAndDestroy();
	TestInsertRemove();

	return 0;
}

/* COMPARE FUNCTIONS */	
	
static int MatchInts(const void *data, const void *user_params)
{
	if (*(int*)data == *(int*)user_params)
	{
		return 1;
	}
	
	return 0;
}	


static size_t HashFunc(const void *key) 
{
    size_t hash_value = 0;
    size_t i = 0;
    size_t table_size = 10;
	char *str = (char *)key;
	
    /* Simple hash function using the ASCII values of characters */
    while (str[i] != '\0') 
    {
        hash_value = (hash_value * 31) + str[i];
        i++;
    }

    /* Modulo to fit the hash value within the size of the hash table */
    return hash_value % table_size;
}

/*********	Test 1	***********/
/* Check for vlg for create and distroy + DListSize + DListIsEmpty*/
static void TestCreateAndDestroy(void)
{
    size_t table_size = 10;
	int i = 0;
	hash_t *hash_ptr = NULL;
	
	for (i = 0; i < NUM_OF_TESTS; i++)
	{
		hash_ptr = HashCreate(table_size, HashFunc, MatchInts);
		
		if ( 0 != HashSize(hash_ptr) ) 
		{
			printf("TestForCreateAndDestroy failed DListSize != 0 for i = %d\n", i);	
		}
		
		if ( 1 != HashIsEmpty(hash_ptr) )
		{
			printf("TestForCreateAndDestroy failed DListIsEmpty != 1 for i = %d\n", i);	
		}
	
		HashDestroy(hash_ptr);
	}
	hash_ptr = NULL;
}


static void TestInsertRemove(void)
{
    size_t table_size = 10;
	size_t i = 0;
	hash_t *hash_ptr = {0};
	int arr_val[NUM_OF_TESTS] = {1, 2, 3, 4, 5, 6, 7, 8};
	char *arr_key[NUM_OF_TESTS] = {"shabso", "gabi", "rostick", "athir",
     								     "marina", "arkadi", "daniel", "koko"};
	
	
	
	hash_ptr = HashCreate(table_size, HashFunc, MatchInts);
			
	for (i = 0; i < NUM_OF_TESTS; i++)
	{
		HashInsert(hash_ptr, arr_key[i], &arr_val[i]);

		if ( (i+1) != HashSize(hash_ptr) ) 
		{
			printf("HashInsert failed HashSize for i = %ld\n", i);	
		}
	}

	for (i = 0; i < NUM_OF_TESTS; i++)
	{
		HashRemove(hash_ptr, arr_key[i]);
		if ( (NUM_OF_TESTS-i) != HashSize(hash_ptr) ) 
		{
/*			printf("HashRemove failed HashSize: for i = %ld\n", i);	
printf("NUM_OF_TESTS-i = %ld, HashSize = %ld\n", (NUM_OF_TESTS-i), HashSize(hash_ptr)); */
		}
		
/*printf("NUM_OF_TESTS-1-i = %ld\n", (NUM_OF_TESTS-1-i));****************************/
	}			
		
	if ( FAIL != HashIsEmpty(hash_ptr) )
	{
		printf("TestForCreateAndDestroy failed DListIsEmpty != 1 for i = %ld\n", i);	
	}	
	
	HashDestroy(hash_ptr);	
	hash_ptr = NULL;
}
















