/*******************************************************************************
* Author: HRD31
* Date: 6.08.2023
* Version: 1
* Description: DHCP server.
*******************************************************************************/
#ifndef __DHCP_H__
#define __DHCP_H__
#include <stddef.h>    /* size_t        */


typedef struct dhcp dhcp_t;
typedef struct dhcp_node dhcp_node_t;
typedef struct ip_address ip_adr_t;

typedef enum status 
{
    SUCCESS, 
    MEMORY_FAIL, 
    ADDRESS_SPACE_FULL
} status_t;

struct ip_address
{
    unsigned char address[4];   
};


/******************************************************************************
Description:     	Creates an dhcp trie.
Return value:    	Pointer to dhcp trie in case of success, otherwise NULL.
Time Complexity: 	O(logn). 
Note:            	Should call "DhcpDestroy()" at end of use.
					
******************************************************************************/
dhcp_t *DhcpCreate(ip_adr_t address, size_t subnet_size);


/******************************************************************************
Description:     	Deletes a dhcp trie pointed by "dhcp" from memory.
Time Complexity: 	O(n). 
Notes:           	Undefined behaviour if 'dhcp' is unvalid.
*******************************************************************************/
void DhcpDestroy(dhcp_t *dhcp);


/*******************************************************************************
Description:     	Allocate  IP address 
Return value:    	In case of success Allocate first empty IP address 
                    if 'preffered_ip' is zero otherwise  'preffered_ip' or 
                    bigger addresses
                    In case of failure will return appropriate status
Time Complexity: 	O(log(n)).
Notes: 			 	Undefined behaviour if trie is invalid pointer.
*******************************************************************************/
status_t DhcpAllocateIp(dhcp_t *dhcp, ip_adr_t preffered_ip, ip_adr_t *result);


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


#endif    
