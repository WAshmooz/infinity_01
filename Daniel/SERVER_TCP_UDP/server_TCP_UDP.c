/*******************************************************************************
****** Author: Daniel Shabso
****** Reviwer: 
****** Date: 23.11.23
****** Version: 1 
****** Description: SERVER- TCP/UDP 
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#include "utils.h" 


#define PORT 8080
#define BUFFER_SIZE 1024

void HandleTCP(int tcp_server_fd, fd_set *master_fds) 
{
    struct sockaddr_in client_addr;
    int new_socket, client_len;
    char buffer[BUFFER_SIZE] = {0};

    if ((new_socket = accept(tcp_server_fd, (struct sockaddr *)&client_addr, 
                                                  (socklen_t*)&client_len)) < 0) 
    {
        perror("TCP accept failed");
        exit(EXIT_FAILURE);
    }

    printf("New TCP connection established\n");

    while (1) 
    {
        int activity = 0;
        struct timeval tv;
        FD_SET(new_socket, master_fds);
        tv.tv_sec = 7;
        tv.tv_usec = 0;
        activity = select(new_socket + 1, master_fds, NULL, NULL, &tv);

        if (activity == -1) 
        {
            perror("Select error");
            exit(EXIT_FAILURE);
        } else if (activity == 0) 
        {
            printf("No activity for 7 seconds. Closing connection.\n");
            break;
        } else {
            if (FD_ISSET(new_socket, master_fds)) 
            {
                int valread = read(new_socket, buffer, BUFFER_SIZE);
                if (valread > 0) 
                {
                    if (strcmp(buffer, "PING") == 0) 
                    {
                        printf("TCP Request: PING\n");
                        send(new_socket, "PONG", strlen("PONG"), 0);
                    } else if (strcmp(buffer, "quit") == 0) 
                    {
                        printf("Server is shutting down by request.\n");
                        exit(EXIT_SUCCESS);
                    }
                } else 
                {
                    printf("TCP client disconnected\n");
                    close(new_socket);
                    break;
                }
            }
        }
    }
}

void HandleUDP(int udp_server_fd, 
                            struct sockaddr_in *client_addr, int *client_len) 
{
    char buffer[BUFFER_SIZE] = {0};

    if (recvfrom(udp_server_fd, buffer, BUFFER_SIZE, 0, 
                    (struct sockaddr *)client_addr, (socklen_t*)client_len) < 0) 
    {
        perror("UDP receive failed");
        exit(EXIT_FAILURE);
    }

    if (strcmp(buffer, "PING") == 0) 
    {
        printf("UDP Request: PING\n");
        sendto(udp_server_fd, "PONG", strlen("PONG"), 0, 
                                (struct sockaddr *)client_addr, *client_len);
    }
}

void RunServer() 
{
    int tcp_server_fd, udp_server_fd;
    struct sockaddr_in server_addr, client_addr;
    int opt = 1, client_len;
    fd_set master_fds; /* array/set of file descriptor */

    FD_ZERO(&master_fds);

    if ((tcp_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    if ((udp_server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) 
    {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(tcp_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
    {
        perror("TCP setsockopt failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(udp_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
    {
        perror("UDP setsockopt failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(tcp_server_fd, (struct sockaddr *)&server_addr, 
                                                    sizeof(server_addr)) < 0) 
    {
        perror("TCP bind failed");
        exit(EXIT_FAILURE);
    }

    if (bind(udp_server_fd, (struct sockaddr *)&server_addr, 
                                                    sizeof(server_addr)) < 0) 
    {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    }

    listen(tcp_server_fd, 3);

    printf("Server listening on port %d\n", PORT);

    while (1) 
    {
        fd_set read_fds = master_fds;
        int max_fd = 0;
        FD_SET(STDIN_FILENO, &read_fds);

        max_fd = (tcp_server_fd > udp_server_fd) ? 
                                                tcp_server_fd : udp_server_fd;
        max_fd = (STDIN_FILENO > max_fd) ? STDIN_FILENO : max_fd;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) 
        {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) 
        {
            char input[BUFFER_SIZE];
            fgets(input, BUFFER_SIZE, stdin);
            input[strcspn(input, "\n")] = 0;
            if (strcmp(input, "PING") == 0) 
            {
                printf("PONG\n");
            } else if (strcmp(input, "quit") == 0) 
            {
                printf("Server is shutting down by request.\n");
                exit(EXIT_SUCCESS);
            }
        }

        if (FD_ISSET(tcp_server_fd, &read_fds)) 
        {
            HandleTCP(tcp_server_fd, &master_fds);
        }

        if (FD_ISSET(udp_server_fd, &read_fds)) 
        {
            client_len = sizeof(client_addr);
            HandleUDP(udp_server_fd, &client_addr, &client_len);
        }
    }
}

int main() 
{
    RunServer();
    return 0;
}
