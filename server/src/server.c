#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "rsa.h"
#include "socket.h"

#define BUFFER_SIZE 2048
#define KEY_SIZE 16 // Size of symetrick key

void handle_client(int client_socket, char *sym_key);
void xor_encrypt_decrypt(char *data, const char *key, size_t length_data);
void generate_random_key(char *key, size_t size);
void print_https_server_banner();
void print_new_client_banner();
void print_client_left_banner();

int main() {
    int port = 8080;
    int server_fd;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Create and inisialize RSA keys
    RSA_PublicKey pub;
    RSA_PrivateKey priv;
    rsa_generate_keys(&pub, &priv);

    print_https_server_banner();

    // Create Server socket
    printf("\n----------------------------\n");
    server_fd = create_tcp_server(port);
    printf("----------------------------\n\n");

    while (1) {
        // Receive connection from client
        int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("Error connection");
            continue;
        }

        print_new_client_banner();

        printf("New client was connected: %s:%d\n\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        
        //---------------------------
        // Sending Public RSA key to Client
        //---------------------------
        write(client_socket, &pub, sizeof(pub));


        //---------------------------
        // Reading Symetrick key from Client
        //---------------------------
        int encrypted_sym_key[KEY_SIZE];
        char decrypted_sym_key[KEY_SIZE];
        read(client_socket, encrypted_sym_key, sizeof(encrypted_sym_key));

        for (int i = 0; i < KEY_SIZE; i++) {
            decrypted_sym_key[i] = rsa_decrypt(encrypted_sym_key[i], priv);
        }

        printf("\n----------  Symetrick key  ----------\n");
        printf("Decrypted symetrick key from Client: %s\n", decrypted_sym_key);


        // Processing client (chat)
        handle_client(client_socket, decrypted_sym_key);

        // Close connection after finish
        close(client_socket);
    }

    close(server_fd);
    return 0;
}

void handle_client(int client_socket, char *sym_key) {
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
        bytes_read = read(client_socket, buffer, message_len);
        if (bytes_read < 0) {
            perror("Error reading from client");
            return;
        } else if (bytes_read == 0) {
            printf("Client disconnected unexpectedly.\n");
            break;
        }

        // Finish line
        buffer[bytes_read] = '\0';

        printf("\n----------  New Message  ----------\n");
        printf("Encrypted from client: %s\n", buffer);
        xor_encrypt_decrypt(buffer, sym_key, message_len);
        printf("Decrypted from client: %s\n", buffer);

        // If client send "exit", close processing
        if (strncmp(buffer, "exit", 4) == 0) {
            print_client_left_banner();
            break;
        }

        // Server's answer
        const char *response = "Hello from server!\n";
        char encrypted_response[BUFFER_SIZE];
        strncpy(encrypted_response, response, BUFFER_SIZE);

        size_t response_len = strlen(response);

        // Message encryption using XOR
        xor_encrypt_decrypt(encrypted_response, sym_key, response_len);

        // Send the length of the encrypted message first
        write(client_socket, &response_len, sizeof(response_len));

        write(client_socket, encrypted_response, response_len);
    }
}

void xor_encrypt_decrypt(char *data, const char *key, size_t length_data) {
    for (size_t i = 0; i < length_data; i++) {
        data[i] ^= key[i % KEY_SIZE]; // Xor with key
    }
}

void print_https_server_banner() {
    printf("\n----------------------------\n");
    printf("  _   _ _____ _____ ____  ____    ____                           \n");
    printf(" | | | |_   _|_   _|  _ \\/ ___|  / ___|  ___ _ ____   _____ _ __ \n");
    printf(" | |_| | | |   | | | |_) \\___ \\  \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\n");
    printf(" |  _  | | |   | | |  __/ ___) |  ___) |  __/ |   \\ V /  __/ |   \n");
    printf(" |_| |_| |_|   |_| |_|   |____/  |____/ \\___|_|    \\_/ \\___|_|   \n");
    printf("                                                                 \n");
    printf("----------------------------\n");
}



void print_new_client_banner() {
    printf("\n----------------------------\n");
    printf("  _   _                  ____ _ _            _     __  \n");
    printf(" | \\ | | _____      __  / ___| (_) ___ _ __ | |_   \\ \\ \n");
    printf(" |  \\| |/ _ \\ \\ /\\ / / | |   | | |/ _ \\ '_ \\| __| (_) |\n");
    printf(" | |\\  |  __/\\ V  V /  | |___| | |  __/ | | | |_   _| |\n");
    printf(" |_| \\_|\\___| \\_/\\_/    \\____|_|_|\\___|_| |_|\\__| (_) |\n");
    printf("                                                   /_/ \n");
    printf("----------------------------\n");
}

void print_client_left_banner() {
    printf("----------------------------\n");
    printf("   ____ _ _            _     _       __ _        __\n");
    printf("  / ___| (_) ___ _ __ | |_  | | ___ / _| |_   _ / /\n");
    printf(" | |   | | |/ _ \\ '_ \\| __| | |/ _ \\ |_| __| (_) | \n");
    printf(" | |___| | |  __/ | | | |_  | |  __/  _| |_   _| | \n");
    printf("  \\____|_|_|\\___|_| |_|\\__| |_|\\___|_|  \\__| (_) | \n");
    printf("                                                \\_\\\n");
    printf("----------------------------\n");
}