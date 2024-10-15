#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket.h"

#define BUFFER_SIZE 2048
#define KEY "mysecretkey" //XOR key

void handle_client(int client_socket);
void xor_encrypt_ecrypt(char *data, const char *key, size_t length_data);

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
    size_t message_len;

    while (1) {
        // Clear bufer
        memset(buffer, 0, sizeof(buffer));

        // First receive the length of the encrypted message
        bytes_read = read(client_socket, &message_len, sizeof(message_len));
        if (bytes_read < 0) {
            perror("Error reading from client");
            return;
        }

        bytes_read = 0;

        // Reading message from client
        bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Error reading from client");
            return;
        }

        if (bytes_read == 0) {
            printf("Client disconnected unexpectedly.\n");
            break;
        }

        // Finish line
        buffer[bytes_read] = '\0';

        xor_encrypt_ecrypt(buffer, KEY, message_len);
        printf("Receive from client: %s\n", buffer);

        // If client send "exit", close processing
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client close session.\n");
            break;
        }

        // Server's answer
        const char *response = "Hello from server!\n";
        char encrypted_response[BUFFER_SIZE];
        strncpy(encrypted_response, response, BUFFER_SIZE);

        size_t response_len = strlen(response);

        xor_encrypt_ecrypt(encrypted_response, KEY, response_len);

        // Send the length of the encrypted message first
        write(client_socket, &response_len, sizeof(response_len));

        write(client_socket, encrypted_response, response_len);
    }
}

void xor_encrypt_ecrypt(char *data, const char *key, size_t length_data) {
    size_t key_len = strlen(key);

    for (size_t i = 0; i < length_data; i++) {
        data[i] ^= key[i % key_len]; // Xor with key
    }
}
