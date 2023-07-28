/*******************************************************************************
****** Author:	 Shabso
****** Reviewer:  
****** Version:  1
****** Date: 	 25.07.2023
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

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
	MyData data1 = {5};
    MyData data2 = {3};
    MyData data3 = {7};
    MyData data4 = {2};
    MyData data5 = {6};
    MyData data6 = {4};
    MyData data7 = {9};
        
    MyData remove_data = {3};
    MyData find_data = {3};
    MyData *found_data = NULL;    

    avl_t *tree = AvlCreate(IsBefore);
    if (tree == NULL) 
    {
        printf("Failed to create AVL tree.\n");
        return;
    }
    
    printf("Print order: 5, 3, 7, 2, 6, 4, 9");	  
            printf("\n\n\n");	      
    /* Insert data into the tree */
    AvlInsert(tree, &data1);
    print2DUtil(tree->root, 0);
        printf("\n\n\n");	    
    AvlInsert(tree, &data2);
    print2DUtil(tree->root, 0);
            printf("\n\n\n");	    
    AvlInsert(tree, &data3);
    print2DUtil(tree->root, 0);
            printf("\n\n\n");	    
    AvlInsert(tree, &data4);
    print2DUtil(tree->root, 0);
            printf("\n\n\n");	    
    AvlInsert(tree, &data5);
    print2DUtil(tree->root, 0);
            printf("\n\n\n");	    
    AvlInsert(tree, &data6);
    print2DUtil(tree->root, 0);
            printf("\n\n\n");	    
    AvlInsert(tree, &data7);
        
print2DUtil(tree->root, 0);
        printf("\n\n\n");	    
    
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

