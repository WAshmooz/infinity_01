/*******************************************************************************
****** Author:	 Shabso
****** Reviewer:  
****** Version:  1
****** Date: 	 20.07.2023
****** Description: Binary Search Tree (recursive) DS header.
*******************************************************************************/
#include <stdlib.h> /* malloc, free, size_t */
#include <assert.h>
#include "avl.h"

/*#ifdef DEBUG*
	#include <stdio.h>
#endif*/


#define COUNT	 5

#define FALSE 0
#define TRUTH 1

#define SUCCESS      0
#define FAIL		 1

#define DATA_EXIST  2

#define LEFT  0
#define RIGHT 1

#define NO_CHILDREN  		0
#define ONE_CHILDREN_LEFT  	1
#define ONE_CHILDREN_RIGHT  2
#define TWO_CHILDREN  		3


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

/******************************************************************************
							Helper functions
******************************************************************************/
/*What is the status children of the node
  Return: 0-NO_CHILDREN, 1-ONE_CHILDREN_LEFT, 2-ONE_CHILDREN_RIGHT, 3-TWO_CHILDREN*/
int StatusChildrens(avl_node_t *what) 
{
	int status = 0;	
	
	if(NULL != what->children[LEFT])
	{
		++status;	
	}
	
	if(NULL != what->children[RIGHT])
	{
		status += 2;
	}
	
	return status;
}

/* Helper function to compare data and get 3 cases*/
static int Compare_Data(avl_node_t *node, is_before_t is_before_func, const void *data)
{
	if (is_before_func(data, node->data) == is_before_func(node->data, data))
	{
		return DATA_EXIST; 	
	}
	
	else if (is_before_func(data, node->data))
	{
		return LEFT; 	
	}
	
	else
	{
		return RIGHT; 	
	}	
}


/* Helper function to update the hiegh of node */
static void UpdateHeight(avl_node_t *node)
{
	size_t left_height = (node->children[LEFT] != NULL) ? node->children[LEFT]->height : 0;
	size_t right_height = (node->children[RIGHT] != NULL) ? node->children[RIGHT]->height : 0;
	node->height = 1 + (left_height > right_height) ? left_height : right_height;
}

/* Helper function to perform a right rotation */
static avl_node_t *RotateRight(avl_node_t *node) 
{
    avl_node_t *new_root = node->children[LEFT];
    node->children[LEFT] = new_root->children[RIGHT];
    new_root->children[RIGHT] = node;
    UpdateHeight(node);
    UpdateHeight(new_root);
    return new_root;
}

/* Helper function to perform a left rotation */
static avl_node_t *RotateLeft(avl_node_t *node) 
{
    avl_node_t *new_root = node->children[RIGHT];
    node->children[RIGHT] = new_root->children[LEFT];
    new_root->children[LEFT] = node;
    UpdateHeight(node);
    UpdateHeight(new_root);
    return new_root;
}

/* Helper function to perform the balancing */
static avl_node_t *Balance_Node(avl_node_t *node, is_before_t is_before_func)
{
	size_t left_height = (node->children[LEFT] != NULL) ? node->children[LEFT]->height : 0;
	size_t right_height = (node->children[RIGHT] != NULL) ? node->children[RIGHT]->height : 0;
	int balance_fac = left_height - right_height;
	
	if (balance_fac > 1) /* left balance */ 
	{
		if (is_before_func(node->children[LEFT]->data, node->data)) 
		{
			/*Left-left case*/
			return RotateRight(node);
		}
		
		else
		{
			/*Left-rigth case*/
			node->children[LEFT] = RotateLeft(node->children[LEFT]);
			return RotateRight(node);
		}
	}
	
	else if (balance_fac < -1)
	{
		if (is_before_func( node->data, node->children[RIGHT]->data)) 
		{
			/*Right-right case*/
			return RotateLeft(node);
		}
		
		else
		{
			/*Left-rigth case*/
			node->children[RIGHT] = RotateLeft(node->children[RIGHT]);
			return RotateLeft(node);
		}
	}			
	
	return node;
}


/******************************************************************************
Description:     	Creates an AVL tree sorted by 'is_before_func'.
Return value:    	Pointer to tree in case of success, otherwise NULL.
Time Complexity: 	O(1).
Note:            	Should call "AvlDestroy()" at end of use
					User must be aware that created tree sored only by 
					'is_before_func'.
******************************************************************************/
avl_t *AvlCreate(is_before_t is_before_func)
{
	avl_t *tree = NULL;
	
	assert(NULL != is_before_func);
	
	tree = (avl_t *)malloc(sizeof(avl_t));
	if (NULL == tree)
	{
		return NULL;
	}
	
	tree->is_before_func = is_before_func;
	tree->root = NULL;
	
	return tree;	
}

/******************************************************************************
Description:     	Deletes a tree pointed to by "tree" from memory.
Time Complexity: 	O(n). 
Notes:           	Undefined behaviour if tree is NULL.
*******************************************************************************/
/* Helper function to recursively destroy the AVL tree */
static void AvlDestroyGo(avl_node_t *node) 
{
    if (node == NULL) 
    {
        return; /* Stop recursion: Nothing to destroy */
    }

    /* Recursively destroy the left and right children */
    AvlDestroyGo(node->children[LEFT]);
    AvlDestroyGo(node->children[RIGHT]);

    /* Free data and node */
    free(node->data);
    node->data = NULL;
    
    free(node);
    node = NULL;
}

void AvlDestroy(avl_t *tree) 
{
    if (tree == NULL) 
    {
        return; /* Nothing to destroy */
    }

    /* Start the recursive destruction from the root of the AVL tree */
    AvlDestroyGo(tree->root);

    free(tree);
    tree = NULL;
}

/*******************************************************************************
Description:     	Deletes the element from the tree pointed to by "what".
Time Complexity: 	O(log(n)).
Notes               Undefined behaviour if no match found.
*******************************************************************************/
static avl_node_t *FindSuccessor(avl_node_t *node)
{
	if(NULL == node->children[LEFT])
	{
		return node;
	}
	
	return FindSuccessor(node->children[LEFT]);
}


static avl_node_t *AvlRemoveGo(avl_node_t *node, void *data, is_before_t is_before)
{
	avl_node_t *temp = NULL;
	if(is_before(data, node->data))
	{
		node->children[LEFT] = AvlRemoveGo(node->children[LEFT], data,
		is_before);
	}
	else if(is_before(node->data, data))
	{
		node->children[RIGHT] = AvlRemoveGo(node->children[RIGHT], data,
		is_before);
	}
	else
	{
		temp = node;
		
		if(NULL != node->children[RIGHT] && NULL != node->children[LEFT])
		{
			temp = FindSuccessor(node->children[RIGHT]);
			node->data = temp->data;
			node->children[RIGHT] = AvlRemoveGo(node->children[RIGHT],
			temp->data, is_before);
		}
		else
		{
			if(NULL == node->children[LEFT])
			{
				node = node->children[RIGHT];
			}
			else if(NULL == node->children[RIGHT])
			{
				node = node->children[LEFT];
			}
			free(temp);
		}
	}
	
	return node;
}

void AvlRemove(avl_t *tree, const void *data)
{
    assert(tree != NULL);
    assert(data != NULL);
    assert(!AvlIsEmpty(tree));

    tree->root = AvlRemoveGo(tree->root, &data, tree->is_before_func);
}
/*******************************************************************************
Description:     	Inserts 'data' to the correct place in 'tree' according to 
					'is_before_func' provided by user at 'AvlCreate' 
Return value:    	0 incase of success otherwise 1.  
Time Complexity: 	O(log(n)), (worst case O(n)).
Notes: 			 	Undefined behaviour if tree is invalid pointer.
*******************************************************************************/
/* Helper function for Inser */
static avl_node_t *AvlInsertGo(avl_node_t *node, void *data, is_before_t is_before_func) 
{
	int compare_result = 0;
	
	/*Create new node with the given data*/
	if (node == NULL)
	{
		avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
		if (new_node == NULL)
		{
			return NULL; 
		}

		new_node->data = data;
		new_node->children[LEFT] = NULL;
		new_node->children[RIGHT] = NULL;
        new_node->height = 1;
        
		return new_node;
	}

	/*Compare data and go left/right/retrun*/
    compare_result = Compare_Data(node, is_before_func, data);

    switch (compare_result)
    {
        case DATA_EXIST: 
            return node;

        case RIGHT:
			node->children[LEFT] = AvlInsertGo(node->children[LEFT], data, is_before_func);
			
			break;

        case LEFT:
            node->children[RIGHT] = AvlInsertGo(node->children[RIGHT], data, is_before_func);
            
			break;
    }
	
	/*Update the height of node*/
	UpdateHeight(node);
	
	/*Balance if necessary*/
	return Balance_Node(node, is_before_func);
	
}

int AvlInsert(avl_t *tree, void *data)
{
    if (tree == NULL || tree->is_before_func == NULL)
    {
        return FAIL;
	}
	
    tree->root = AvlInsertGo(tree->root, data, tree->is_before_func);
    
    return SUCCESS;
}
/*******************************************************************************
Description:     	Returns 'tree' height.
Time Complexity: 	O(log(n)).
Notes:			 	Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
size_t AvlHeight(const avl_t *tree)
{
	return tree->root->height;
}

/*******************************************************************************
Description:     	Returns number of elements in "tree".
Time Complexity: 	O(n).
Notes:			 	Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
/* Helper function for size */
static size_t AvlSizeGo(avl_node_t *root)
{
	size_t leftSize = 0;
	size_t rightSize = 0;
	
    if (NULL == root) 
    {
        return 0;
    }

	leftSize = AvlSizeGo(root->children[LEFT]);
	rightSize = AvlSizeGo(root->children[RIGHT]);

	return 1 + leftSize + rightSize;
}

size_t AvlSize(const avl_t *tree)
{
    return AvlSizeGo(tree->root);
}

/*******************************************************************************
Description: 		Checks if "tree" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
int AvlIsEmpty(const avl_t *tree)
{
	if (NULL == tree->root)
	{
		return TRUTH;
	}
	
	return FALSE;	
}

/*******************************************************************************
Description:  	  	Calls "action_func" on all elements in tree until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n log n)
Notes:            	User responsible that 'action_func' should not affect sort 
					order.
*******************************************************************************/
static int AvlForEachPreIMP(avl_node_t *node, action_func_t action_func,
															  void *user_params)
{
	if (node == NULL)
	{
		return SUCCESS;
	}
	
	if (FAIL == action_func(node->data, user_params))
	{
		return FAIL;
	}
	
	AvlForEachPreIMP(node->children[LEFT], action_func, user_params);
	AvlForEachPreIMP(node->children[RIGHT], action_func, user_params);
	
	return SUCCESS;
}				  

int AvlForEachPreorder(avl_t *tree, action_func_t action_func, void *user_params)
{
	assert(NULL != tree);
	assert(NULL != action_func);
	
	return AvlForEachPreIMP(tree->root, action_func, user_params);
}


/*******************************************************************************
Description:  	  	Calls "action_func" on all elements in tree until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n log n)
Notes:            	User responsible that 'action_func' should not affect sort 
					order.
*******************************************************************************/
static int AvlForEachInIMP(avl_node_t *node, action_func_t action_func,
															  void *user_params)
{	
	if (node == NULL)
	{
		return SUCCESS;
	}
	
	AvlForEachInIMP(node->children[LEFT], action_func, user_params);


	if (FAIL == action_func(node->data, user_params))
	{
		return FAIL;
	}

	AvlForEachInIMP(node->children[RIGHT], action_func, user_params);
	
	return SUCCESS;
	
}

int AvlForEachInorder(avl_t *tree, action_func_t action_func, void *user_params)
{
	assert(NULL != tree);
	assert(NULL != action_func);
	
	return AvlForEachInIMP(tree->root, action_func, user_params);
}

/*******************************************************************************
Description:  	  	Calls "action_func" on all elements in tree until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n log n)
Notes:            	User responsible that 'action_func' should not affect sort 
					order.
*******************************************************************************/
static int AvlForEachPostIMP(avl_node_t *node, action_func_t action_func,
															  void *user_params)
{
	if (node == NULL)
	{
		return SUCCESS;
	}
	
	/*Go all the way LEFT and return when NULL*/	
	AvlForEachPostIMP(node->children[LEFT], action_func, user_params);
	
	/*Go RIGHT and return when NULL, else Go left all the way, Start over*/	
	AvlForEachPostIMP(node->children[RIGHT], action_func, user_params);
	
	if (FAIL == action_func(node->data, user_params))
	{
		return FAIL;
	}
	else
	{
		return SUCCESS;
	}
}

int AvlForEachPostorder(avl_t *tree, action_func_t action_func,
															  void *user_params)
{
	assert(NULL != tree);
	assert(NULL != action_func);
	
	return AvlForEachPostIMP(tree->root, action_func, user_params);
}

/*******************************************************************************
Description:		Finds the first match according to "is_before_func" 
Return value:       Pointer to data incase of success, otherwise NULL.
Time complexity:    O(log n)
Note:          		Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
void *AvlFindGo(avl_node_t *node, void *data, is_before_t is_before_func)
{
    int compare_result = 0;

    if (node == NULL)
    {
        return NULL;
    }

    compare_result = Compare_Data(node, is_before_func, data);

    switch (compare_result)
    {
        case DATA_EXIST: 
            return node;

        case LEFT:
            return AvlFindGo(node->children[LEFT], data, is_before_func);

        case RIGHT:
            return AvlFindGo(node->children[RIGHT], data, is_before_func);
    }

    return NULL; /* Handle cases where the comparison result is not valid */
}

void *AvlFind(const avl_t *tree, const void *data)
{
	is_before_t is_before_func = tree->is_before_func;
			
    return AvlFindGo(tree->root, (void *)data, is_before_func);
}




