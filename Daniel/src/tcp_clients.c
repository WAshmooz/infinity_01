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
/*TCP clients*/

int main(int argc, char *argv[]) 
{

    char *server_ip = argv[1];
    int port = atoi(argv[2]);
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_MSG_LEN];
    const char *message = "Hello, Server!";

    if (argc < 3) 
    {
        printf("Usage: %s <server_ip> <port_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0); /*SOCK_DGRAM*/
    if (sockfd < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    /*Dont need "connect" in UDP*/
    if (connect(sockfd, (struct sockaddr *)&server_addr, 
                                                       sizeof(server_addr)) < 0) 
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    /* Send messages to the server */
    
    /*This line for UCP insteade of the "send"/
    /*if (sendto(sockfd, message, strlen(message), 0, 
                    (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)*/ 
    if (send(sockfd, message, strlen(message), 0) < 0) 
    {
        perror("Error sending message");
        exit(EXIT_FAILURE);
    }

    printf("Message sent to server.\n");

    close(sockfd);

    return 0;
}
