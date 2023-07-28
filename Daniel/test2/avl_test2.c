/*******************************************************************************
****** Author:	 Shabso
****** Reviewer:  
****** Version:  1
****** Date: 	 25.07.2023
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "../include/avl.h"
void AvlPrint(avl_t *tree);

#define COUNT 5

/* User-defined data structure */
typedef struct 
{
    int value;
} MyData;

typedef struct avl_node avl_node_t;

struct avl
{
	avl_node_t *root;
	is_before_t is_before_func;
};

struct avl_node
{
    void *data;
    avl_node_t *children[2];
    size_t height; 
};

static int IsBefore(const void *lhs, const void *rhs)
{
	return *(int *)lhs < *(int *)rhs;
}

/* Function to print MyData elements */
int PrintMyData(void *data, void *user_params) 
{
    MyData *d = (MyData *)data;
    printf("Value: %d\n",d->value);
    return 0;
}

void print2DUtil(avl_node_t *root, int space)
{
   int i = COUNT;
    if (root == NULL)
        return;
 
    space += COUNT;
 
    print2DUtil(root->children[1], space);
 
    for (i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", *(int *)root->data);
 
    print2DUtil(root->children[0], space);
}

/* Test function for the AVL tree */
void test_avl_tree() 
{
	int dataArr[] = { 5, 3 ,7 ,2 ,6 ,4 ,9, 13, 14 };
	const size_t SIZE = sizeof(dataArr) / sizeof(int);
	MyData remove_data = {3};
	MyData find_data = {3};
	MyData *found_data = NULL;    
	size_t i = 0;
	
	avl_t *tree = AvlCreate(IsBefore);
	if (tree == NULL) 
	{
		printf("Failed to create AVL tree.\n");
		return;
	}
	
	printf("Print order: ");
	for (i = 0; i < SIZE; ++i) {
		printf("%d, ", dataArr[i]);
	}
	printf("\n\n\n");
	
	/* Insert data into the tree */
	for (i = 0; i < SIZE; ++i) {
		AvlInsert(tree, &dataArr[i]);
		/* print2DUtil(tree->root, 0); */
		AvlPrint(tree);
		/* printf("--------------------------\n"); */
	}
	
	
	return;

    /* Print the tree in-order */
    printf("In-order traversal:\n");
    AvlForEachInorder(tree, PrintMyData, NULL);
    /* Find an element in the tree */
    found_data = AvlFind(tree, &find_data);
    if (found_data != NULL) 
    {
        printf("Found data: %d\n", found_data->value);
    } 
    
    else 
    {
        printf("Data not found.\n");
    }
printf("Exit point 2\n");      
    /* Remove an element from the tree */
    AvlRemove(tree, &remove_data);
printf("Exit point 3\n");    
    /* Print the tree in-order after removal */
    printf("In-order traversal after removal:\n");
    AvlForEachInorder(tree, PrintMyData, NULL);
    /* Cleanup */
    AvlDestroy(tree);
}

int main() 
{
    test_avl_tree();
    
    return 0;
}

