#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket.h"

#define BUFFER_SIZE 1024

void handle_client(int client_socket);

int main() {
    int port = 8080;
    int server_fd;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    
    server_fd = create_tcp_server(port);

    printf("Server is working and wait for connection...\n");

    while (1) {
        // Receive connection from client
        int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("Error connection");
            continue;
        }

        printf("New client was connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Processing client (chat)
        handle_client(client_socket);

        // Close connection after finish
        close(client_socket);
    }

    close(server_fd);
    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while (1) {
        // Clear bufer
        memset(buffer, 0, sizeof(buffer));

        // Reading message from client
        bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Error reading from client");
            return;
        }

        // Finish line
        buffer[bytes_read] = '\0';
        printf("Receive from client: %s\n", buffer);

        // If client send "exit", close processing
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client close session.\n");
            break;
        }

        // Server's answer
        const char *response = "Hello from server!\n";
        write(client_socket, response, strlen(response));
    }
}
