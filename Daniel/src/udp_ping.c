/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer: 
****** Date: 20.11.23
****** Version: 1 
****** Description: TCP / UDP Ping-Pong (ex 1 - UDP SERVER)
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "utils.h" /*RETURN_IF_ERROR, ERR_SYS*/

#define MAX_MSG_LEN 100


int main() 
{
    int sockfd = 0, bytes_received = 0, bytes_sent = 0, iter = 0;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_MSG_LEN] = {0};
    socklen_t addr_size = 0;

    /*Creating socket file descriptor*/
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    RETURN_IF_ERROR(sockfd < 0, "Socket creation failed", ERR_SYS);

    initializeSocketAddress(&server_addr);

    RETURN_IF_ERROR(bind(sockfd, (struct sockaddr *)&server_addr, 
                           sizeof(server_addr)) < 0, "Binding failed", ERR_SYS);

    addr_size = sizeof(client_addr);

    while (iter < 10)
    {
        /*Receive from client*/
        bytes_received = recvfrom(sockfd, buffer, MAX_MSG_LEN, 0, 
                                   (struct sockaddr *)&client_addr, &addr_size);
        RETURN_IF_ERROR(0 > bytes_received, "recvfrom", ERR_SYS);

        printf("Client: %s\n", buffer);
        sleep(1);

        /*Reply with "PONG"*/
        strcpy(buffer, "PONG");
        bytes_sent = sendto(sockfd, buffer, strlen(buffer), 0, 
                              (const struct sockaddr *)&client_addr, addr_size);
        RETURN_IF_ERROR(0 > bytes_sent, "sendto", ERR_SYS);
        
        /*printf("Server: PONG\n");
        sleep(1);*/

        ++iter;
    }

    close(sockfd);

    return 0;
}
