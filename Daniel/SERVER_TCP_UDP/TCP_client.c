/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer: 
****** Date: 23.11.23
****** Version: 1 
****** Description: Client- TCP 
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

void RunTCPClient() 
{
    int tcp_client_fd = -1, status = -1, pings = -1, i = 0;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    srand(time(NULL));

    /*Creating socket file descriptor*/
    tcp_client_fd = socket(AF_INET, SOCK_STREAM, 0);
    ExitIfError(tcp_client_fd < 0, "TCP socket creation failed", ERR_SYS);

    initializeSocketAddress(&server_addr, PORT);

    /*Convert IPv4 and IPv6 addresses from text to binary and store in BUFF*/
    status = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    ExitIfError(status <= 0, "Invalid address/ Address not supported",
                                                                    ERR_SYS);

    /*Open a connection on socket*/
    status = connect(tcp_client_fd, (struct sockaddr *)&server_addr, 
                                                           sizeof(server_addr));
    ExitIfError(status < 0, "Connection failed",ERR_SYS);

    LogAction("Connected to server");

    pings = rand() % 8 + 3; /*Random number of pings 3-8*/
    for (i = 0; i < pings; ++i) 
    {
        int bytes_sent = -1, bytes_received = -1;

        sleep(rand() % 8 + 3); /*Random sleep 3-10 seconds*/

        bytes_sent = send(tcp_client_fd, "PING", strlen("PING"), 0);
        ExitIfError(0 > bytes_sent, "send failed", ERR_SYS);

        LogAction("Sent PING");

        bytes_received = recv(tcp_client_fd, buffer, BUFFER_SIZE, 0);
        ExitIfError(0 > bytes_received, "recvfrom failed", ERR_SYS);
        if (0 == strcmp(buffer, "PONG")) 
        {
            LogAction("Received PONG");
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(tcp_client_fd);
    LogAction("Disconnected from server");
}

int main() 
{
    RunTCPClient();
    return 0;
}
