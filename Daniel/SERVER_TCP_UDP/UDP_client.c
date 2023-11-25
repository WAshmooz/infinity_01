/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer: 
****** Date: 23.11.23
****** Version: 1 
****** Description: Client- UDP 
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#include "utils.h" 

#define SERVER_IP "127.0.0.1" /*Server's IP address*/
#define PORT 8080
#define BUFFER_SIZE 1024

void RunUDPClient() 
{
    int udp_client_fd = -1, pings = -1, i = 0;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    srand(time(NULL));

    if ((udp_client_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) 
    {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) 
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    LogAction("UDP Client started");

    pings = rand() % 8 + 3; /*Random number of pings between 3 and 10*/
    for (i = 0; i < pings; ++i) 
    {
        socklen_t addr_length = sizeof(server_addr);

        sleep(rand() % 8 + 3); /*Random sleep between 3 and 10 seconds*/

        sendto(udp_client_fd, "PING", strlen("PING"), 0, 
                    (const struct sockaddr *)&server_addr, sizeof(server_addr));
        LogAction("Sent PING");

        if (recvfrom(udp_client_fd, buffer, BUFFER_SIZE, 0, 
                            (struct sockaddr *)&server_addr, &addr_length) > 0) 
        {
            if (strcmp(buffer, "PONG") == 0) 
            {
                LogAction("Received PONG");
            }
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(udp_client_fd);
    LogAction("UDP Client stopped");
}

int main() 
{
    RunUDPClient();
    return 0;
}
