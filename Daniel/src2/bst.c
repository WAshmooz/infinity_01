/***************************************************************************
Author: Daniel Shabso
Reviewer: 
Date of Creation: 16.7.23
Last date of update:   
version: 1
****************************************************************************/
#include <stdlib.h>  /* malloc, free, size_t */
#include <assert.h> /* assert */
#include "bst.h"


#define FALSE 0
#define TRUTH 1

#define SUCCESS      0
#define FAIL		 1

#define LEFT  0
#define RIGHT 1

#define NO_CHILDREN  		0
#define ONE_CHILDREN_LEFT  	1
#define ONE_CHILDREN_RIGHT  2
#define TWO_CHILDREN  		3


#define NODE(itr) 			itr.node


struct tree_node
{
    void *data;
    tree_node_t *parent;
    tree_node_t *children[2];
};

struct bst
{
    tree_node_t *root;
    is_before_t is_before_func;
};

/******************************************************************************
Description:     	Assign a node to itr
******************************************************************************/
static bst_itr_t MakeItr(tree_node_t *node)
{
	bst_itr_t itr_to_return;
	itr_to_return.node = node;
	
	return itr_to_return;
}
/******************************************************************************
    					Helper functions
******************************************************************************/
/*Is the node Rigth children?*/
int IsHeRigthChild(bst_itr_t what)
{
	/*TRUTH = 1 , FALSE = 0*/
	return NODE(what) == NODE(what)->parent->children[RIGHT] ? TRUTH : FALSE;	
}

int IsHeGotLeftChild(bst_itr_t what)
{
	/*TRUTH = 1 , FALSE = 0*/
	return NULL == NODE(what)->children[LEFT] ? FALSE : TRUTH;	
}

int IsHeGotRigthChild(bst_itr_t what)
{
	/*TRUTH = 1 , FALSE = 0*/
	return NULL == NODE(what)->children[RIGHT] ? FALSE : TRUTH;	
}

/*What is the status children of the node
  Return: 0-NO_CHILDREN, 1-ONE_CHILDREN_LEFT, 2-ONE_CHILDREN_RIGHT, 3-TWO_CHILDREN*/
int StatusChildrens(bst_itr_t what) 
{
	int status = 0;	
	
	if(NULL != NODE(what)->children[LEFT])
	{
		++status;	
	}
	
	if(NULL != NODE(what)->children[RIGHT] )
	{
		status += 2;
	}
	
	return status;
}

/*Go back Left one step */
void GoBackOneStepLeft(bst_itr_t what) 
{
	if(IsHeRigthChild(what) == TRUTH)
	{
		NODE(what) = NODE(what)->parent;
	}
}

/*Go back Rigth one step */
void GoBackOneStepRigth(bst_itr_t what) 
{
	if(IsHeRigthChild(what) == FALSE)
	{
		NODE(what) = NODE(what)->parent;
	}
}

/*Go down Left one step */
void GoDownOneStepLeft(bst_itr_t what) 
{
	if(IsHeGotLeftChild(what) == TRUTH)
	{
		NODE(what) = NODE(what)->children[LEFT];
	}
}

/*Go down Rigth one step */
void GoDownOneStepRigth(bst_itr_t what) 
{
	if(IsHeGotRigthChild(what) == TRUTH)
	{
		NODE(what) = NODE(what)->children[RIGHT];
	}
}

/*Go back Left all the way */
void GoBackAllTheWayLeft(bst_itr_t what) 
{
	while(IsHeRigthChild(what) == TRUTH)
	{
		NODE(what) = NODE(what)->parent;
	}
}

/*Go back Rigth all the way */
void GoBackAllTheWayRigth(bst_itr_t what) 
{
	while(IsHeRigthChild(what) == FALSE)
	{
		NODE(what) = NODE(what)->parent;
	}
}

/*Go down Left all the way */
void GoDownAllTheWayLeft(bst_itr_t what) 
{
	while(IsHeGotLeftChild(what) == TRUTH)
	{
		NODE(what) = NODE(what)->children[LEFT];
	}
}

/*Go down Rigth all the way */
void GoDownAllTheWayRigth(bst_itr_t what) 
{
	while(IsHeGotRigthChild(what) == TRUTH)
	{
		NODE(what) = NODE(what)->children[RIGHT];
	}
}

/******************************************************************************
Description:     	Creates a binary search tree sorted by 'is_before_func'.
Return value:    	Pointer to tree in case of success, otherwise NULL.
Time Complexity: 	O(1). 
Note:            	Should call "BstDestroy()" at end of use
					User must be aware that created tree sored only by 
					'is_before_func'
******************************************************************************/
bst_t *BstCreate(is_before_t is_before_func)
{
	bst_t *bst = (bst_t *)malloc(sizeof(bst_t));
	if(NULL == bst)
	{
		return NULL;
	}

	bst->root = (tree_node_t *)malloc(sizeof(tree_node_t));
	if(NULL == bst->root)
	{
		free(bst);
		bst = NULL;
		return NULL;
	}	
	
	bst->is_before_func = is_before_func;

	bst->root->data = NULL;		
	bst->root->parent = NULL;	
	bst->root->children[LEFT] = NULL;	
	bst->root->children[RIGHT] = NULL;
	
	return bst;	
}

/******************************************************************************
Description:     	Deletes a tree pointed by "tree" from memory.
Time Complexity: 	O(n). 
Notes:           	Undefined behaviour if tree is NULL.
*******************************************************************************/
void BstDestroy(bst_t *tree)
{
	tree_node_t *current = NULL, *prev = NULL;
	
	assert(NULL != tree);
	
	current = tree->root->children[LEFT];
	
	/*While tree is not Empty*/	
	while(FALSE == BstIsEmpty(tree))
	{
		/* While we didint got to node without children */
		while(NO_CHILDREN != StatusChildrens(MakeItr(current)))
		{
			/* Move left if you can */
			if(TRUTH == IsHeGotLeftChild(MakeItr(current)))
			{
				prev = current;
				current = current->children[LEFT];
			}
			
			/* Else move rigth if you can*/
			else if(TRUTH == IsHeGotRigthChild(MakeItr(current)))
			{
				prev = current;
				current = current->children[RIGHT];
			}			
		}
		
		/* Arrived a node without children */
		free(current);
		current = prev;	
		prev = current->parent;
	}
	
	free(tree->root);
	tree->root = NULL;
	
	free(tree);
	tree = NULL;
}

/*******************************************************************************
Description:     	Inserts 'data' to the correct place in 'tree' according to 
					'sort_func' provided by user at 'BstCreate' 
Return value:    	Iterator to inserted data on success otherwise NULL/
					BstIterEnd().   
Time Complexity: 	O(log(n)), (worst case O(n)).
Notes: 			 	Undefined behaviour if tree is invalid pointer.
*******************************************************************************/
bst_itr_t BstInsert(bst_t *tree, void *data)
{
	tree_node_t *current = NULL, *prev = NULL, *new_node = NULL;
	is_before_t is_before_func = NULL;
	int is_rigth_children = FALSE;
	void *curr_data = NULL;	
	
	assert(NULL != tree);
	
	/*Malloc for new node*/ 
	new_node = (tree_node_t *)malloc(sizeof(tree_node_t));
	if (NULL == new_node)
	{
		return BstIterEnd(tree);
	}
	
	/*First we initialize current(firs node) and prev(root)*/
	current = tree->root->children[LEFT];
	prev = tree->root;
	is_before_func = tree->is_before_func;
	
	/*Go in the tree according to is_before_func until you got to NULL*/
	while(NULL != current)
	{
		prev = current;
		curr_data = BstGetData(MakeItr(current));
	
		if (is_before_func(data, curr_data))
		{
			current = current->children[LEFT];
			is_rigth_children = FALSE;
		}
		
		else
		{
			current = current->children[RIGHT];
			is_rigth_children = TRUTH;
		}	
	}
	
	/*We got to NULL so we need to go back to parent(prev)
	  and initialize data and childrens*/
	new_node->data = data;		
	new_node->parent = prev;
	new_node->children[LEFT] = NULL;	
	new_node->children[RIGHT] = NULL;	

	/*Insert node to the rigth children*/
	prev->children[is_rigth_children] = new_node;
			
	return MakeItr(new_node);
}

/*******************************************************************************
Description:     	Deletes the element from the tree pointed to by "what".
Return value:    	Iterator to next element.
Time Complexity: 	O(log(n))
Notes:         		Undefined behaviour if "what" is out of tree's 
                    range or BstIterEnd() 
*******************************************************************************/
bst_itr_t BstRemove(bst_itr_t what)
{
    tree_node_t *what_node = NODE(what);
    bst_itr_t next;
    void *temp_data = NULL;


	assert(what_node != NULL);
	
    /* Find the next element after the one we are going to remove */
    next = BstIterNext(what);

    switch (StatusChildrens(what))
    {
        case NO_CHILDREN:
        
                /* Remove node with no children */
                what_node->parent->children[IsHeRigthChild(what)] = NULL;
            break;
        
        case ONE_CHILDREN_LEFT:
        
                /* Remove node with only left child */
                what_node->parent->children[IsHeRigthChild(what)] = what_node->children[LEFT];
                what_node->children[LEFT]->parent = what_node->parent;
            
            break;
        
        case ONE_CHILDREN_RIGHT:
        
 				/* Remove node with only right child */
                what_node->parent->children[IsHeRigthChild(what)] = what_node->children[RIGHT];
                what_node->children[RIGHT]->parent = what_node->parent;

            break;
        
        case TWO_CHILDREN:
        
            /* Find the next element after the one we are going to remove */
            next = BstIterNext(what);

            /* Swap data between the node and its next element */
            temp_data = what_node->data;
            what_node->data = NODE(next)->data;
            NODE(next)->data = temp_data;

            /* Now remove the next node which is a leaf or has one right child */
            if (StatusChildrens(next) == NO_CHILDREN || StatusChildrens(next) == ONE_CHILDREN_RIGHT)
            {
                what_node->parent->children[IsHeRigthChild(next)] = NODE(next)->children[RIGHT];
                
                if (NODE(next)->children[RIGHT])
                {
                    NODE(next)->children[RIGHT]->parent = what_node->parent;
                }
            }
            break;
        
    }

    /* Free the memory of the removed node */
    free(what_node);

    return next;
}

/*******************************************************************************
Description:     	Returns number of elements in "tree".
Time Complexity: 	O(n).
Notes:			 	Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
size_t BstSize(const bst_t *tree)
{
    size_t size = 0;
    bst_itr_t itr, end;
    
    assert(NULL != tree);
	
    itr = BstIterBegin(tree);
    end = BstIterEnd(tree);

    while (FALSE == BstIterIsEqual(itr, end))
    {
        ++size;
        itr = BstIterNext(itr);
    }

    return size;	
}

/*******************************************************************************
Description: 		Checks if "tree" is empty.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
Note: 				Undefined behaviour if 'list' is invalid pointer.
*******************************************************************************/
int BstIsEmpty(const bst_t *tree)
{
	assert(NULL != tree);
	
	if (NULL == tree->root->children[LEFT])
	{
		return TRUTH;
	}
	
	return FALSE;
		
}

/*******************************************************************************
Description:  	  	Calls "action_func" on each element between [from, to)
					("to" is exclusive) until fail.
Return value:     	0 for success, 1 for fail.
Time complexity:  	O(n log n)
Notes:            	Undefined behavior if "from" or "to" are not valid or do
					not belong to the same tree.
					User responsible that 'action_func' should not affect sort 
					order.  
*******************************************************************************/
int BstForEach(bst_itr_t from, bst_itr_t to, action_func_t action_func,
				 void *user_params)
{
	assert(NULL != action_func);
	assert(NULL != user_params);

    while (!BstIterIsEqual(from, to))
    {
    	if(FAIL == action_func(BstGetData(from), user_params))
    	{
			return FAIL;
        }
        
        from = BstIterNext(from);        
    }

    return SUCCESS;	
}


/*******************************************************************************
Description:		Finds the first match according to "is_before_func" 
Return value:   	Iterator to first match in tree if found, otherwise
                    BstIterEnd().
Time complexity:    O(log n)
Note:          		Undefined behaviour if 'tree' is invalid pointer.
*******************************************************************************/
bst_itr_t BstFind(const bst_t *tree, const void *data)
{
	bst_itr_t curr;
	is_before_t is_before_fun = NULL;
		
	assert(NULL != tree);
	assert(NULL != data);	

	/*Start from the first node */ 	
	curr = MakeItr(tree->root->children[LEFT]);
	is_before_fun = tree->is_before_func;	
	
	while (NULL != NODE(curr))
	{
		if (is_before_fun(data, BstGetData(curr)))
		{

			curr = MakeItr(NODE(curr)->children[LEFT]);
			continue;
		}
		
		if (!is_before_fun(BstGetData(curr), data))
		{
			return curr;
		}
		curr = MakeItr(NODE(curr)->children[RIGHT]);
	}
	
	return BstIterEnd(tree);
}

/*******************************************************************************
Description:  		Returns next iterator of "itr".
Time complexity:  	O(log(n)) ,(worst case O(n)).
Note:    			Undefined behaviour if "itr" is BstIterEnd().
*******************************************************************************/
bst_itr_t BstIterNext(bst_itr_t itr)
{
    int status = 0;
	    
	assert(NULL != NODE(itr));
	
	status = StatusChildrens(itr);
	
    switch (status)
    {
        case NO_CHILDREN:
            /*SAME 1*/
            GoBackAllTheWayLeft(itr);
            GoBackOneStepRigth(itr);
                   		
            break;
			

        case ONE_CHILDREN_LEFT:
        	/*SAME 1*/
            GoBackAllTheWayLeft(itr);
            GoBackOneStepRigth(itr);
        	
            break;
         
       case ONE_CHILDREN_RIGHT:
			/*SAME 2*/
        	GoDownOneStepRigth(itr);
        	GoDownAllTheWayLeft(itr);
			
            break;
          
          
        case TWO_CHILDREN:
        	/*SAME 2*/
        	GoDownOneStepRigth(itr);
        	GoDownAllTheWayLeft(itr);        	
        	          
            break;
    }   
    
 	return itr;  
}        	
/*******************************************************************************
Description:  		Returns the iterator before "itr".
Time complexity:  	O(log(n)) ,(worst case O(n)).
Note:    			Undefined behaviour if "itr" BstIterBegin() 
*******************************************************************************/
bst_itr_t BstIterPrev(bst_itr_t itr)
{
    int status = 0;
	    
	assert(NULL != NODE(itr));
	
	status = StatusChildrens(itr);
	
    switch (status)
    {
        case NO_CHILDREN:
            /*SAME 1*/
            GoBackAllTheWayRigth(itr);
            GoBackOneStepLeft(itr);
                   		
            break;
			

        case ONE_CHILDREN_LEFT:
        	/*SAME 2*/
            GoDownOneStepLeft(itr);
            GoDownAllTheWayRigth(itr);
        	
            break;
         
       case ONE_CHILDREN_RIGHT:
			/*SAME 1*/
        	GoBackAllTheWayRigth(itr);
        	GoBackOneStepLeft(itr);
			
            break;
          
          
        case TWO_CHILDREN:
        	/*SAME 2*/
            GoDownOneStepLeft(itr);
            GoDownAllTheWayRigth(itr);      	
        	          
            break;
    }   
    
 	return itr;  
} 
/*******************************************************************************
Description: 		Returns an iterator to the first valid element.   
Time complexity:  	O(log(n)), (worst case O(n)).
Note:        		Undefined behaviour if "tree" is invalid pointer
*******************************************************************************/
bst_itr_t BstIterBegin(const bst_t *tree)
{
	bst_itr_t begin = MakeItr(tree->root);
	
	assert(NULL != tree);
	
	GoDownAllTheWayLeft(begin);  
	  	
	return begin;
}

/*******************************************************************************
Description:   		Returns an iterator to the dummy end (invalid element) 
Time complexity:  	O(1).
Note: 				Undefined behaviour if "tree" is invalid pointer
*******************************************************************************/
bst_itr_t BstIterEnd(const bst_t *tree)
{
	assert(NULL != tree);
	
	return MakeItr(tree->root);
}

/*******************************************************************************
Description: 		Checks if iter1 and iter2 are iterators to the same 
					element.
Return value:   	1 for true, 0 for false.
Time complexity:  	O(1)
*******************************************************************************/
int BstIterIsEqual(bst_itr_t itr1, bst_itr_t itr2)
{
	assert(NULL != NODE(itr1));
	assert(NULL != NODE(itr2));
	
	return NODE(itr1) == NODE(itr2);	
}

/*******************************************************************************
description:     	Returns pointer to data at 'itr'.
Time Complexity: 	O(1)
Notes:           	Undefined behaviour if 'itr' is out of tree range.
*******************************************************************************/
void *BstGetData(const bst_itr_t itr)
{
	assert(NULL != NODE(itr));
	
	return itr.node->data;	
}


