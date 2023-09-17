/*******************************************************************************
****** Author: HRD31
******	Author: 	Daniel Shabso
******	Reviewer: 	Michael 
****** Date: 30.05.2023
****** Version: 1
*******************************************************************************/

/**********************		Headers		***************************************/

#include <stdio.h>  	/* printf, size_t 	*/
#include <assert.h> 	/* assert        	*/
#include <string.h> 	/* memcpy        	*/
#include <time.h>  		/* time_t 			*/
#include <unistd.h> 	/* pid_t  			*/
#include <ifaddrs.h> 	/* getifaddrs    	*/ 
#include <arpa/inet.h>  /* INET6_ADDRSTRLEN */
#include <pthread.h>    /*pthread_mutex_t*/

#include "uid.h"


/******************************************************************************/

const Uid_t UID_BAD = {0, 0, 0, "0"};

/******************************************************************************
Description:     	Creates an UID.
Return value:    	uid_t on success, UID_BAD on fail.
Time Complexity: 	O(1).         	
******************************************************************************/
static pthread_mutex_t uid_mutex = PTHREAD_MUTEX_INITIALIZER;

Uid_t UidCreate(void)
{
	static size_t count_st = 0;
	Uid_t uid;
    char ip[INET6_ADDRSTRLEN] = {0};
    struct ifaddrs *ifaddr, *ifa;

    /*Lock the mutex to make this function thread-safe*/
    pthread_mutex_lock(&uid_mutex);
	uid.pid = getpid();
	uid.timestamp = time(NULL);
	uid.count = count_st;
	++count_st;
	
    if (uid.timestamp == -1) 
    {
        /*Unlock the mutex before returning on error*/
        pthread_mutex_unlock(&uid_mutex);
		return UID_BAD;
    }	

    if (getifaddrs(&ifaddr) == -1) 
    {
        /*Unlock the mutex before returning on error*/
        pthread_mutex_unlock(&uid_mutex);
		return UID_BAD;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) 
        {
            if (inet_ntop(ifa->ifa_addr->sa_family, 
                          (ifa->ifa_addr->sa_family == AF_INET) ? 
                           (void *)&(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr) : 
                            (void *)&(((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr), ip, INET6_ADDRSTRLEN) != NULL) 
            {
                /* Concatenate the IP address to the unique identifier */
            } 
        }
    }
  
    memcpy(uid.ip, ip, IP_SIZE);
    freeifaddrs(ifaddr);

    /*Unlock the mutex before returning*/
    pthread_mutex_unlock(&uid_mutex); 
    
    return uid;
}

/******************************************************************************
Description:     	Compares between two UIDs.
Return value:    	1 for true, 0 for false.
Time Complexity: 	O(1).
******************************************************************************/
int UidIsEqual(Uid_t uid1, Uid_t uid2)
{
	if(uid1.count == uid2.count && uid1.pid == uid2.pid && uid1.timestamp == uid2.timestamp && strcmp(uid1.ip, uid2.ip) == 0) 
	{
		return 1;
	}	
	
	return 0;	
}



















