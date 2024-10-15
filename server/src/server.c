#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include "socket.h"

#define BUFFER_SIZE 2048
#define KEY_SIZE 16 // Size of symetrick key

void handle_client(int client_socket);
void xor_encrypt_ecrypt(char *data, const char *key, size_t length_data);
void generate_random_key(char *key, size_t size);

int main() {
    int port = 8080;
    int server_fd;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    
    server_fd = create_tcp_server(port);
    srand(time(NULL));

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
    char key[KEY_SIZE];
    int bytes_read;
    size_t message_len;

    generate_random_key(key, KEY_SIZE);
    write(client_socket, key, KEY_SIZE);
    printf("Generated and send key to client: %s\n", key);

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
        bytes_read = read(client_socket, buffer, message_len);
        if (bytes_read < 0) {
            perror("Error reading from client");
            return;
        }

        printf("Length of message: %lu\n", message_len);

        if (bytes_read == 0) {
            printf("Client disconnected unexpectedly.\n");
            break;
        }

        // Finish line
        buffer[bytes_read] = '\0';

        xor_encrypt_ecrypt(buffer, key, message_len);
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

        xor_encrypt_ecrypt(encrypted_response, key, response_len);

        // Send the length of the encrypted message first
        write(client_socket, &response_len, sizeof(response_len));

        write(client_socket, encrypted_response, response_len);
    }
}

void xor_encrypt_ecrypt(char *data, const char *key, size_t length_data) {
    for (size_t i = 0; i < length_data; i++) {
        data[i] ^= key[i % KEY_SIZE]; // Xor with key
    }
}

void generate_random_key(char *key, size_t size) {
    for (size_t i = 0; i < size; i++) {
        key[i] = 'A' + (rand() % 26);  // Random symbol from alfabet
    }
    key[size] = '\0';
}
