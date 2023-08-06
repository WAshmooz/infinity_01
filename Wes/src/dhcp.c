/*******************************************************************************
* Author: HRD31
* Date: 6.08.2023
* Version: 1
* Description: DHCP server.
*******************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include "../include/dhcp.h" 


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
	dhcp_node_t *current = {0};
	
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


int main ()
{
	return 0;
}


