#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void chat(int client_socket) {
    char buffer[BUFFER_SIZE];

    while (1) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));

        // Input message to client
        printf("Input message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to server
        write(client_socket, buffer, strlen(buffer));

        // If was wrotten "exit", finish the program
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exit from chat...\n");
            break;
        }

        // Clear buffer for receiving the answer
        memset(buffer, 0, sizeof(buffer));

        // Receive answer from server
        read(client_socket, buffer, sizeof(buffer));

        // Print received message
        printf("Answer from server: %s\n", buffer);
    }
}

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
