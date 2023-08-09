/*******************************************************************************
* Author: HRD31
* Date: 6.08.2023
* Version: 1
* Description: DHCP server.
*******************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include "../include/stack.h" 
#include "../include/dhcp.h" 

/***** only for debug *********/
#include <stdio.h>			
#include "../include/aux_funcs.h"		
#include "../include/vector.h"			

#define IP_SIZE    32
#define BLOCK_SIZE 2
#define FAIL_IN_C (-1)
/*typede enum status 
{
    SUCCESS, 
    MEMORY_FAIL, 
    ADDRESS_SPACE_FULL
} status_t;*/
  

struct dhcp
{
    ip_adr_t subnet_id;
    dhcp_node_t *root; 
  	size_t subnet_size; 
};

struct dhcp_node
{
    int is_full;
    dhcp_node_t *node[BLOCK_SIZE];
};

enum { FREE = 0, FULL = 1 };
enum { false = 0, true = 1 };

/******************************************************************************
						Helper Functions
******************************************************************************/
/*Function to get the value of a specific bit at the given index*/
int GetBit(ip_adr_t ip, size_t index) 
{
	if (index >= 32) 
	{
		return FAIL_IN_C; 
	}
	else 
	{
		size_t byteIndex = index / 8;
		size_t bitPosition = index % 8;
		
		unsigned char byte_value = ip.address[byteIndex];
		int bit_value = (byte_value >> (7 - (bitPosition))) & 1;
    	
		return bit_value;
	}
}




/*Creat nodes till the end of tree. if allocation failed it dosent free the 
  nodes that allocated ! */
status_t CraeteNodesForIp(dhcp_node_t *current, ip_adr_t ip, size_t startIdx)
{
	size_t i = 0;
	int bit;
	while (startIdx < 32)
	{
		bit = GetBit(ip, startIdx++);
		if (NULL == current->node[bit]) {
			current->node[bit] = (dhcp_node_t *)malloc(sizeof(dhcp_node_t));
			if (NULL == current->node[bit])
			{
				return MEMORY_FAIL;
			}
			current->node[bit]->is_full = FREE;
		}
		
		current = current->node[bit];
	}
	
	current->is_full = FULL;
	return SUCCESS;
}


status_t CraeteNodesTillReachEnd(dhcp_node_t *current, size_t tree_hight_left, int bit)
{
	size_t i = 0;
	for (i = 0; i < tree_hight_left; i++)
	{
		current->node[bit] = (dhcp_node_t *)malloc(sizeof(dhcp_node_t));
		if (NULL == current->node[bit])
		{
			return MEMORY_FAIL;
		}
		
		current->node[bit]->is_full = FREE;
		bit = 0;
		current = current->node[bit];
		
	}
	
	/* current->node[bit]->is_full = FULL; */
	current->is_full = FULL;
	return SUCCESS;
}




/******************************************************************************
Description:     	Creates an dhcp trie.
Return value:    	Pointer to dhcp trie in case of success, otherwise NULL.
Time Complexity: 	O(logn). 
Note:            	Should call "DhcpDestroy()" at end of use.
					
******************************************************************************/
dhcp_t *DhcpCreate(ip_adr_t address, size_t subnet_size)
{
	int bit = 0;
	size_t i = 0;
	const size_t tree_hight = IP_SIZE - subnet_size;
	dhcp_t *dhcp;
	dhcp_node_t *current = { 0 };
	
	/* Check if subnet_size is within valid range (1 to 32 bits) */
	if (subnet_size == 0 || subnet_size > IP_SIZE)
	{
		return NULL;
	}
	
	dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
	if (dhcp == NULL)
	{
		return NULL; /*Memory allocation failed*/
	}
	
	dhcp->subnet_id = address;
	dhcp->subnet_size = subnet_size;
    
	dhcp->root = (dhcp_node_t *)malloc(sizeof(dhcp_node_t));
	if (dhcp->root == NULL)
	{
		free(dhcp);
		return NULL; /* Memory allocation failed*/
	}
    
	/*Initialize the root node*/
	dhcp->root->is_full = FREE;
	dhcp->root->node[0] = NULL;
	dhcp->root->node[1] = NULL;
	
	/*Now we Allocate implicitly what we were asked*/
	current = dhcp->root;
	
	/* build 000's left */
	for (bit = 0; bit <= 1; ++bit)
	{
		current = dhcp->root;
		for (i = 0; i < tree_hight; i++)
		{
			current->node[bit] = (dhcp_node_t *)malloc(sizeof(dhcp_node_t));
			if (NULL == current->node[bit])
			{
				DhcpDestroy(dhcp);
				return NULL;    /* Memory allocation failed, Destroy and return NULL*/
			}
			
			current->node[bit]->is_full = FREE;
			current->node[bit]->node[0] = NULL;
			current->node[bit]->node[1] = NULL;
			
			current = current->node[bit];
		}
		current->is_full = 1;
	}
	
	current = dhcp->root;
	for (i = 0; i < tree_hight - 1; i++)
	{
		current = current->node[1];
	}
	
	assert(current->node[1] != NULL);
	assert(current->node[0] == NULL);
	
	bit = 0;
	current->node[bit] = (dhcp_node_t *)malloc(sizeof(dhcp_node_t));
	if (current->node[bit] == NULL)
	{
		DhcpDestroy(dhcp);
		return NULL;         /* Memory allocation failed, Destroy and return NULL*/
	}
	
	current->is_full = 1;
	
	current->node[bit]->is_full = 1;
	current->node[bit]->node[0] = NULL;
	current->node[bit]->node[1] = NULL;
	
	
	
	return dhcp;
}

/******************************************************************************
Description:     	Deletes a dhcp trie pointed by "dhcp" from memory.
Time Complexity: 	O(n). 
Notes:           	Undefined behaviour if 'dhcp' is unvalid.
*******************************************************************************/
/* Helper function to recursively destroy the AVL tree */
static void DhcpDestroyGo(dhcp_node_t *current) 
{
    if (current == NULL) 
    {
        return; /* Stop recursion: Nothing to destroy */
    }

    /* Recursively destroy the children */
    DhcpDestroyGo(current->node[0]);
    DhcpDestroyGo(current->node[1]);
    
    free(current);
    current = NULL;
}

void DhcpDestroy(dhcp_t *dhcp)
{
    if (dhcp == NULL) 
    {
        return; /* Nothing to destroy */
    }

    /* Start the recursive destruction from the root  */
    DhcpDestroyGo(dhcp->root);

    free(dhcp);
    dhcp = NULL;
}


/*******************************************************************************
Description:     	Allocate  IP address 
Return value:    	In case of success Allocate first empty IP address 
                    if 'preffered_ip' is zero otherwise  'preffered_ip' or 
                    bigger addresses
                    In case of failure will return appropriate status
Time Complexity: 	O(log(n)).
Notes: 			 	Undefined behaviour if trie is invalid pointer.
*******************************************************************************/
status_t DhcpAllocateIp(dhcp_t *dhcp, ip_adr_t preffered_ip, ip_adr_t *result)
{
	const size_t tree_hight = IP_SIZE - (dhcp->subnet_size);
	size_t i = 0;
	size_t index_of_bit = (IP_SIZE - tree_hight);
	status_t status= ADDRESS_SPACE_FULL;
	dhcp_node_t *current = dhcp->root; 
	stack_t *stack_parents =  StackCreate(tree_hight, sizeof(dhcp_node_t*));
	void *parnet_node =  NULL;
	int bit = GetBit(preffered_ip, index_of_bit);
	int isIpExist = 1;
	
	*result = preffered_ip;
	
	/*First push the root to stack*/
	parnet_node = &current;
	StackPush(stack_parents, parnet_node);
	
	printf("tree_hight = %d\n", tree_hight);
	for (i = 0; i < tree_hight; i++)
	{
		bit = GetBit(preffered_ip, (index_of_bit + i));
		printf("bit = %x\n", bit);
		/*if there is no nodes we create the ip nodes we need*/
		if (NULL == current->node[bit])
		{
			isIpExist = 0;
			printf("new node creation\n"); 
			current->node[bit] = (dhcp_node_t *)malloc(sizeof(dhcp_node_t*));
			if (NULL == current->node[bit])
			{
				return MEMORY_FAIL; /* TODO: free memory */
			}
			current->node[bit]->is_full = FREE;
			current->node[bit]->node[0] = NULL;
			current->node[bit]->node[1] = NULL;
			/* CraeteNodesTillReachEnd(current, (tree_hight - i), bit); */
			/* return status; */
		}
		current = current->node[bit];
		
		/*if next node free, push to stack and go to child*/
		if (FREE == current->is_full)
		{
			parnet_node = &current;
			StackPush(stack_parents, parnet_node);
		}
		else break;
	}
	
	if (0 == isIpExist) {
		current = *(dhcp_node_t**)StackPeek(stack_parents);
		StackPop(stack_parents);
		current->is_full = FULL;
		while (StackSize(stack_parents)) {
			current = *(dhcp_node_t**)StackPeek(stack_parents);
			StackPop(stack_parents);
			if (current == dhcp->root) break;
			
			if (current->node[0]->is_full && current->node[1]->is_full) {
				current->is_full = FULL;
			}
		}
		return status;
	}
	else  /* we need to suggest other option */
	{
		;
	}
	
	return status;
	/*TODO update result and status*/
}
/*******************************************************************************
Description:     	Free the ip address passed in 'ip_to_free'.
Time Complexity: 	O(log(n))
Notes:         		Undefined behaviour if "dhcp" is invalid pointer 
*******************************************************************************/
void DhcpFreeIp(dhcp_t *dhcp, ip_adr_t ip_to_free);


/*******************************************************************************
Description:     	Returns number of free ip address.
Time Complexity: 	O(n).
Notes:			 	Undefined behaviour if "dhcp" is invalid pointer.
*******************************************************************************/
size_t DhcpCountFree(const dhcp_t *dhcp);












/*******************************************************************************
					TEST FUNCTIONS + MAIN 
*******************************************************************************/
static void PrintNode(const void *element)
{
	const dhcp_node_t **elem = (const dhcp_node_t **)element;
	
	if (*elem)
		printf("%2d", (*elem)->is_full);
	else
		printf("  ");
}

static void PrintBTreeFromVector(vector_t *vec, PrintElement printFunc)
{
	int i = 0, j = 0, level = 0;
	int curIdx = 0;
	int nodes_on_level = 1;
	void *curNode;
	int safeStop = 100;
	int treeSpaces1[] = {
		126, 0,
		62,  126,
		30, 62,
		14, 30,
		6, 14,
		2,  6,
		0,  2
	};
	int treeSpaces2[] = {
		62,  0,
		30, 62,
		14, 30,
		6, 14,
		2,  6,
		0,  2
	};

	int *treeSpaces = treeSpaces2;
	int spacesBefore, spacesInBetween;
	size_t vectorSize = VectorSize(vec);
	
	while (--safeStop && curIdx < vectorSize)
	{
		spacesBefore = treeSpaces[level++];
		spacesInBetween = treeSpaces[level++];
		printf("%*s", spacesBefore, "");
		
		for (i = 0; i < nodes_on_level && curIdx < vectorSize; ++i, ++curIdx)
		{
			curNode = VectorGetAccess(vec, curIdx);
			printFunc(curNode);
			
			if (i % 2) {
				printf("%*s", spacesInBetween, "");
			}
			else {
				for (j = 0; j < spacesInBetween; ++j) printf("-");
				/* printf("%-s", half-4, ""); */
			}
		}
		nodes_on_level *= 2;
		printf("\n");
	}
	printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
static void TreeToVector(vector_t *vec, dhcp_node_t *root)
{
	size_t i;
	size_t vecSize = VectorSize(vec);
	dhcp_node_t **elem = (dhcp_node_t**)VectorGetAccess(vec, 0);
	dhcp_node_t **elem2;
	
	*elem = root;
	
	/*printf("%u %p\n", (int)vecSize, *elem);*/
	
	for (i = 0; i < vecSize / 2 - 1; ++i) {
		elem = (dhcp_node_t**)VectorGetAccess(vec, i);
		
		/*printf("%ld %p\n", i, *elem);*/
		elem2 = (dhcp_node_t**)VectorGetAccess(vec, 2 * i + 1);
		if (*elem) *elem2 = (*elem)->node[0];
		else       *elem2 = NULL;
		
		elem2 = (dhcp_node_t**)VectorGetAccess(vec, 2 * i + 2);
		if (*elem) *elem2 = (*elem)->node[1];
		else       *elem2 = NULL;
	}
}
static void PrintTree(dhcp_t *dh, vector_t *vec)
{
	TreeToVector(vec, dh->root);
	PrintBTreeFromVector(vec, PrintNode);
}

static void printCharBits(const unsigned char ch)
{
	int i;
	for (i = 0; i < 8; i++) {
		printf("%d", !!((ch << i) & 0x80));
	}
}
static void printIpAddress(ip_adr_t ip) 
{
    printf("%d.%d.%d.%d", ip.address[0], ip.address[1], ip.address[2], ip.address[3]);
    printf(" == %02X.%02X.%02X.%02X  ", ip.address[0], ip.address[1], ip.address[2], ip.address[3]);
    printCharBits(ip.address[3]);
    printf("\n");
}

static void printGetBit(ip_adr_t ip, size_t index) 
{
	size_t i = 0;
	int git_bit = 0;
printf("\nTEST FOR GetBit(index = %ld):\n", index);
	for (i = (int)index; i <= 32; i++)
	{
		git_bit = GetBit(ip, i);
		printf("\nGetBit(%ld) = %d\n", i, git_bit);
	}
}

/* this should pow of 2, because the vector will store tree nodes pointers 
* so it can be 64, 128, 256 
*/
#define VECTROR_CAPACITY 128



int main()
{
	/*********	TEST CREAT + DESTROY	*********/
	ip_adr_t address = { 0xaa, 0x88, 0xFF, 205 }; /*205 = 11001101*/
	ip_adr_t res; /*205 = 11001101*/
	dhcp_t *dh = DhcpCreate(address, 28);
	vector_t *vec = VectorCreate(VECTROR_CAPACITY, sizeof(void*));
	int i = 0, git_bit = 0;
	void *Stam = NULL;
	size_t index_for_test_helper_fun = 0;
printIpAddress(dh->subnet_id);
printIpAddress(address);
	for (i = 0; i < VECTROR_CAPACITY; ++i) 
	{
		VectorPushBack(vec, &Stam);
	}
	
	PrintTree(dh, vec);
	address.address[3] = 0x0D; /* 1101 */
	DhcpAllocateIp(dh, address, &res);
	PrintTree(dh, vec);
	
	/*
	   | 0000 | V |       | 1000 |
	   | 0001 |   |       | 1001 |
	   | 0010 |   |       | 1010 |
	   | 0011 |   |       | 1011 |
	   | 0100 |   |       | 1100 |
	   | 0101 |   |       | 1101 |
	   | 0110 |   |       | 1110 | V
	   | 0111 |   |       | 1111 | V
	*/
	
	
	address.address[3] = 0x0D; /* 1101 */
	CraeteNodesForIp(dh->root, address, 28);
	address.address[3] = 0x07; /* 0111 */
	CraeteNodesForIp(dh->root, address, 28);
	PrintTree(dh, vec);
	
	printf("End\n");
	
	DhcpDestroy(dh);
	VectorDestroy(vec);
	
	/*********	TEST HELPER FUNCTIONS	*********/
	index_for_test_helper_fun = 27;
	printGetBit(address, index_for_test_helper_fun);
	
	return 0;
}



