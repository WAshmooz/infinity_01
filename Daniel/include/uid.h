/*******************************************************************************
****** Author: HRD31
******	Date: 29.06.2023
******	Version: 1
******	Description: header file for UID data structure.
*******************************************************************************/
#ifndef __UID_H__
#define __UID_H__

#include <stddef.h> /* size_t */
#include <time.h>   /* time_t */
#include <sys/types.h>

#define IP_SIZE 10

typedef struct uid /*DO NOT USE!*/
{
    size_t count;
    pid_t pid;
    time_t timestamp;
    char ip[IP_SIZE];
} Uid_t;

extern const Uid_t UID_BAD;

/******************************************************************************
Description:     	Creates an UID.
Return value:    	uid_t on success, UID_BAD on fail.
Time Complexity: 	O(1).         	
******************************************************************************/
Uid_t UidCreate(void);

/******************************************************************************
Description:     	Compares between two UIDs.
Return value:    	1 for true, 0 for false.
Time Complexity: 	O(1).
******************************************************************************/
int UidIsEqual(Uid_t uid1, Uid_t uid2);

#endif    /*__UID_H__*/
