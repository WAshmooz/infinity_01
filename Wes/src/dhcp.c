/*******************************************************************************
* Author: HRD31
* Date: 6.08.2023
* Version: 1
* Description: DHCP server.
*******************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/dhcp.h" 
#include "../include/aux_funcs.h"
#include "../include/vector.h"


#define BLOCK_SIZE 2
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
/******************************************************************************
Description:     	Creates an dhcp trie.
Return value:    	Pointer to dhcp trie in case of success, otherwise NULL.
Time Complexity: 	O(logn). 
Note:            	Should call "DhcpDestroy()" at end of use.
					
******************************************************************************/
dhcp_t *DhcpCreate(ip_adr_t address, size_t subnet_size)
{
	size_t i = 0;
	const size_t remaining_bits = 32 - subnet_size;
	dhcp_t *dhcp;
	dhcp_node_t *current = { 0 };
	
	/* Check if subnet_size is within valid range (1 to 32 bits) */
	if (subnet_size == 0 || subnet_size > 32)
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
	dhcp->root->is_full = 0;
	dhcp->root->node[0] = NULL;
	dhcp->root->node[1] = NULL;
	
	/*Now we Allocate implicitly what we were asked*/
	current = dhcp->root;
	
	/* build 000's left */
	int bit = 0;
	for (bit = 0; bit <= 1; ++bit)
	{
		current = dhcp->root;
		for (i = 0; i < remaining_bits; i++)
		{
			current->node[bit] = (dhcp_node_t *)malloc(sizeof(dhcp_node_t));
			if (current->node[bit] == NULL)
			{
				DhcpDestroy(dhcp);
				return NULL;              /* Memory allocation failed, Destroy and return NULL*/
			}
			
			current->node[bit]->is_full = 0;
			current->node[bit]->node[0] = NULL;
			current->node[bit]->node[1] = NULL;
			
			current = current->node[bit];
		}
		current->is_full = 1;
	}
	
	current = dhcp->root;
	for (i = 0; i < remaining_bits - 1; i++)
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
		return NULL;              /* Memory allocation failed, Destroy and return NULL*/
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
void DhcpDestroy(dhcp_t *dhcp)
{
	;
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
	
	printf("%u %p\n", vecSize, *elem);
	
	for (i = 0; i < vecSize / 2 - 1; ++i) {
		elem = (dhcp_node_t**)VectorGetAccess(vec, i);
		
		printf("%d %p\n", i, *elem);
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


/* this should pow of 2, because the vector will store tree nodes pointers 
* so it can be 64, 128, 256 
*/
#define VECTROR_CAPACITY 128
int main()
{
	ip_adr_t address = { 0xaa, 0x88, 0xFF, 0x00 };
	dhcp_t *dh = DhcpCreate(address, 28);
	vector_t *vec = VectorCreate(VECTROR_CAPACITY, sizeof(void*));
	int i = 0;
	void *Stam = NULL;
	for (i = 0; i < VECTROR_CAPACITY; ++i) {
		VectorPushBack(vec, &Stam);
	}
	
	PrintTree(dh, vec);
	
	printf("End\n");
	
	
	return 0;
}


