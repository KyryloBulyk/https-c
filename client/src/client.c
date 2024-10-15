#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include "rsa.h"

#define BUFFER_SIZE 2048
#define KEY_SIZE 16 // Size of symetrick key

void chat(int client_socket, const char *key);
void xor_encrypt_decrypt(char *data, const char *key, size_t length_data);
void generate_random_key(char *key, size_t size);
void print_https_client_banner();

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char key[KEY_SIZE];

    // Set base for generating
    srand(time(NULL));

    print_https_client_banner();

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

    printf("\nConnecting to server\n");


    //-------------------------
    // Receive RSA and send to server
    //-------------------------
    RSA_PublicKey rsa_pub_key;
    read(client_socket, &rsa_pub_key, sizeof(RSA_PublicKey));
    printf("RSA public key received from server\n");

    generate_random_key(key, KEY_SIZE);
    printf("\n----------  Generating symmetrick key  ----------\n");
    printf("Generated symmetric key: %s\n", key);

    int encrypted_sym_key[KEY_SIZE];
    for (int i = 0; i < KEY_SIZE; i++) {
        encrypted_sym_key[i] = rsa_encrypt(key[i], rsa_pub_key);
    }

    write(client_socket, encrypted_sym_key, sizeof(encrypted_sym_key));
    printf("Encrypted symmetric key sent to server\n\n");


    // Starting chat
    chat(client_socket, key);

    // Closing the socket after completion
    close(client_socket);
    return 0;
}

void chat(int client_socket, const char *key) {
    char buffer[BUFFER_SIZE];
    char encrypted_message[BUFFER_SIZE];
    size_t encrypted_len;

    while (1) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
        memset(encrypted_message, 0, sizeof(encrypted_message));

        // Input message to client
        printf("\n---------- New Message ----------\n");
        printf("Input message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        strncpy(encrypted_message, buffer, BUFFER_SIZE);

        size_t message_len = strlen(buffer);

        xor_encrypt_decrypt(encrypted_message, key, strlen(encrypted_message));

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

        printf("\n----------  Server's Answer  ----------\n");
        printf("Encrypted Answer from server: %s\n", buffer);

        xor_encrypt_decrypt(buffer, key, encrypted_len);

        // Print received message
        printf("Answer from server: %s\n", buffer);
    }
}

void xor_encrypt_decrypt(char *data, const char *key, size_t length_data) {
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

void print_https_client_banner() {
    printf("--------------------------------\n");
    printf("  _   _ _____ _____ ____  ____     ____ _ _            _   \n");
    printf(" | | | |_   _|_   _|  _ \\/ ___|   / ___| (_) ___ _ __ | |_ \n");
    printf(" | |_| | | |   | | | |_) \\___ \\  | |   | | |/ _ \\ '_ \\| __|\n");
    printf(" |  _  | | |   | | |  __/ ___) | | |___| | |  __/ | | | |_ \n");
    printf(" |_| |_| |_|   |_| |_|   |____/   \\____|_|_|\\___|_| |_|\\__|\n");
    printf("                                                           \n");
    printf("--------------------------------\n");
}
