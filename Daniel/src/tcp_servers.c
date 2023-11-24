#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_MSG_LEN 100

int main(int argc, char *argv[]) 
{

    int port = atoi(argv[1]);
    int sockfd;
    int bytes_received = 0;
    struct sockaddr_in server_addr;
    char buffer[MAX_MSG_LEN];

    if (argc < 2) 
    {
        printf("Usage: %s <port_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*Creating a Socket*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0); /*SOCK_DGRAM*/
    if (sockfd < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    /*Initializing Server Address Structure*/
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /*Binding the Socket*/
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    /*Listening for Incoming Connections*/
    if (listen(sockfd, 5) < 0)                  /*dont need to listen in udp*/
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    while (1) 
    {
        /*Accepting Connections and Receiving Messages*/
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        int connfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
        if (connfd < 0) 
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted from %s:%d\n", 
                  inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* Receive messages from clients (Handling Accepted Connection)*/
        bytes_received = recv(connfd, buffer, sizeof(buffer), 0);
        if (bytes_received < 0) 
        {
            perror("Error receiving message");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0';
        printf("Received message on port %d: %s\n", port, buffer);

        close(connfd);
    }

    /*Closing the Socket*/
    close(sockfd);

    return 0;
}

    /*while (1) /*replace the while condition dor udp "
    {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                    (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received < 0) 
        {
            perror("Error receiving message");
            exit(EXIT_FAILURE);
        }

        buffer[bytes_received] = '\0';
        printf("Received message on port %d: %s\n", port, buffer);
    }*/
