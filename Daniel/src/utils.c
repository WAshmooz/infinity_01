/*******************************************************************************
****** Author: HRD31
****** Date: 14.08.2023
****** Version: 1
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include <errno.h>
#include <stdarg.h>

#include "utils.h"

void ExitIfError(int condition, const char *msg, int err_type)
{
	if (condition)
	{
        fprintf(stderr, "Error: %s (Line %d)\n", msg, __LINE__); 
		exit(err_type);
	}
}

void initializeSocketAddress(struct sockaddr_in *sockaddr) 
{
    uint16_t port = 8080;

    /* Clear the sockaddr structure */
    memset(sockaddr, 0, sizeof(*sockaddr));

    /*Set the address family to IPv4*/
    sockaddr->sin_family = AF_INET;
    /*Allow the socket to listen on any available network interface*/
    sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    /*Set the port number (8080) for the server*/
    sockaddr->sin_port = htons(port);
}

void Err_N_Die(const char *fmt, ...)
{
    int errno_save;
    va_list ap;

    errno_save = errno;

    /*print out the fmt+args to standart out*/
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    /*print out error message is errno was set*/
    if(errno_save != 0)
    {
        fprintf(stdout, "(errno = %d) : %s\n", errno_save, strerror(errno_save));
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    va_end(ap);

    exit(1);
}
/*(SOCK_DGRAM/SOCK_STREAM , &sockaddr*/
/*int MySocket_AFINIT(int __type, struct sockaddr_in *sockaddr) 
{
    int sockfd = socket(AF_INET, __type, 0);
    ExitIfError(sockfd < 0, "Socket creation failed", ERR_SYS);
    return sockfd;
}*/