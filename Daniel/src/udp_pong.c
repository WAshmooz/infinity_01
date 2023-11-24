/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer: 
****** Date: 20.11.23
****** Version: 1 
****** Description: TCP / UDP Ping-Pong (ex 1 - UDP CLIENT)
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "utils.h" /*ExitIfError, ERR_SYS*/

#define MAX_MSG_LEN 100

int main() 
{
    int sockfd = 0, bytes_received = 0, bytes_sent = 0, iter = 0;
    struct sockaddr_in server_addr;
    char buffer[MAX_MSG_LEN] = {0};

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ExitIfError(sockfd < 0, "Socket creation failed", ERR_SYS);

    initializeSocketAddress(&server_addr);


    while (iter < 10) 
    { 
        strcpy(buffer, "PING");
        bytes_sent = sendto(sockfd, buffer, strlen(buffer), 0,
                    (const struct sockaddr *)&server_addr, sizeof(server_addr));
        ExitIfError(0 > bytes_sent, "sendto", ERR_SYS);

        /*printf("Client: PING\n");
        sleep(1);*/

        memset(buffer, '\0', sizeof(buffer));
        bytes_received = recvfrom(sockfd, buffer, MAX_MSG_LEN, 0, NULL, NULL);
        ExitIfError(0 > bytes_received, "recvfrom", ERR_SYS);

        printf("Server: %s\n", buffer);
        sleep(1);
        
        ++iter;
    }

    printf("Ping-Pong iterations: %d\n", iter);

    close(sockfd);

    return 0;
}
