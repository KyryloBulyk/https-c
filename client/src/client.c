#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 2048
#define KEY "mysecretkey" //XOR key

void chat(int client_socket);
void xor_encrypt_ecrypt(char *data, const char *key, size_t length_data);

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // Create TCP-socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Set adress server
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connecting to server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection to server failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connecting to server!\n");

    // Starting chat
    chat(client_socket);

    // Closing the socket after completion
    close(client_socket);
    return 0;
}

void chat(int client_socket) {
    char buffer[BUFFER_SIZE];
    char encrypted_message[BUFFER_SIZE];
    size_t encrypted_len;

    while (1) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
        memset(encrypted_message, 0, sizeof(encrypted_message));

        // Input message to client
        printf("Input message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        strncpy(encrypted_message, buffer, BUFFER_SIZE);

        size_t message_len = strlen(buffer);

        xor_encrypt_ecrypt(encrypted_message, KEY, strlen(encrypted_message));

        // Send the length of the encrypted message first
        write(client_socket, &message_len, sizeof(message_len));

        // Send message to server
        write(client_socket, encrypted_message, message_len);

        // If was wrotten "exit", finish the program
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exit from chat...\n");
            break;
        }

        // Clear buffer for receiving the answer
        memset(buffer, 0, sizeof(buffer));

        // First receive the length of the encrypted message
        read(client_socket, &encrypted_len, sizeof(encrypted_len));

        // Receive answer from server
        read(client_socket, buffer, encrypted_len);

        xor_encrypt_ecrypt(buffer, KEY, encrypted_len);

        // Print received message
        printf("Answer from server: %s\n", buffer);
    }
}


void xor_encrypt_ecrypt(char *data, const char *key, size_t length_data) {
    size_t key_len = strlen(key);

    for (size_t i = 0; i < length_data; i++) {
        data[i] ^= key[i % key_len]; // Xor with key
    }
}