/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer: 
****** Date: 20.11.23
****** Version: 1 
****** Description: TCP / UDP Ping-Pong (ex 3 - Brodcast)
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>/*setsockopt*/
#include <sys/types.h>/*setsockopt*/
#include <errno.h>

#include "utils.h" /*RETURN_IF_ERROR, ERR_SYS*/

#define PORT 8080
#define MAX_MSG_LEN 100
/*Change this to your network's broadcast address - "ifconfig" in the terminal*/
#define BROADCAST_ADDR "192.168.199.255" 

int main() 
{
    int sockfd = -1;
    int broadcast_enable = 1;
    struct sockaddr_in broadcast_addr;
    char buffer[MAX_MSG_LEN] = "Hello World";

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    RETURN_IF_ERROR(sockfd < 0, "Socket creation failed", ERR_SYS);


    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_ADDR);

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, 
                                                  sizeof(broadcast_enable)) < 0) 
    {
        perror("Setting broadcast option failed");
        exit(EXIT_FAILURE);
    }
    
    if (sendto(sockfd, buffer, strlen(buffer), 0, 
            (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0) 
    {
        perror("Error sending message");
        exit(EXIT_FAILURE);
    }

    printf("Sent broadcast message: %s\n", buffer);

    close(sockfd);

    return 0;
}

