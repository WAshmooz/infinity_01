/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer: 
****** Date: 20.11.23
****** Version: 1 
****** Description: TCP / UDP Ping-Pong (ex 2 - TCP SERVER)
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
    int sockfd = -1, connfd = -1;
    int bytes_received = 0;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_MSG_LEN];
    socklen_t addr_size = 0;
    const char *reply = NULL;

    /* Create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ExitIfError(sockfd < 0, "Socket creation failed", ERR_SYS);


    /* Set server address struct */
    initializeSocketAddress(&server_addr);

    /* Bind socket */
    ExitIfError(bind(sockfd, (struct sockaddr *)&server_addr, 
                          sizeof(server_addr)) < 0, "Binding failed", ERR_SYS);

    /* Listen for incoming connections */
    ExitIfError((listen(sockfd, 5)) < 0, "Listen failed", ERR_SYS);

    addr_size = sizeof(client_addr);

    /* Accept connections and handle communication */
    connfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
    ExitIfError(0 > connfd, "Server accept failed", ERR_SYS);

    /* Communication loop */
    while (1)  
    {        
        memset(buffer, 0, MAX_MSG_LEN); /*Erases data by writing zeros*/
        bytes_received = read(connfd, buffer, sizeof(buffer));
        ExitIfError(bytes_received < 0, "Error reading from socket", 
                                                                       ERR_SYS);
        if (bytes_received == 0) 
        {
            printf("Connection closed by client\n");
            break;
        }
        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "PING") == 0) 
        {
            reply = "PONG";
            if (write(connfd, reply, strlen(reply)) < 0) 
            {
                perror("Error writing to socket");
                break;
            }
        }
    }

    close(connfd);
    close(sockfd);

    return 0;
}


