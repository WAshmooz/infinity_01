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

#include "utils.h" /*RETURN_IF_ERROR, ERR_SYS*/

#define MAX_MSG_LEN 100
#define SERVER_IP "127.0.0.1" 

int main() 
{
    int sockfd = -1, bytes_received = -1, iter = 0;
    struct sockaddr_in server_addr;
    char buffer[MAX_MSG_LEN] = {0};
    const char *msg = NULL;

    /* Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    RETURN_IF_ERROR(sockfd < 0, "Socket creation failed", ERR_SYS);

    /* Set server address struct */
    initializeSocketAddress(&server_addr);


    /* Convert IPv4 and IPv6 addresses from text to binary form */
    RETURN_IF_ERROR(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0, 
                             "Invalid address/ Address not supported", ERR_SYS);

    /* Connect to server */
    RETURN_IF_ERROR(connect(sockfd, (struct sockaddr *)&server_addr, 
                    sizeof(server_addr)) < 0, "Connection failed", ERR_SYS);

    printf("Connected to server\n");

    /* Communication loop */
    for (iter = 0; iter < 10; ++iter)
    {
        msg = "PING";
        /*printf("Sending: %s\n", msg);
        sleep(1);*/

        /* Send message to server */
        if (send(sockfd, msg, strlen(msg), 0) < 0) 
        {
            perror("Error sending message");
            break;
        }

        bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
        RETURN_IF_ERROR(bytes_received < 0, "Error receiving message", ERR_SYS);

        buffer[bytes_received] = '\0';
        printf("Client: %s\n", buffer);
        sleep(1);
    }

    close(sockfd);

    return 0;
}

