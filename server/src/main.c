#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "socket.h"

#define BUFFER_SIZE 1024

void handle_client(SSL *ssl);

int main() {
    int port = 8080;
    int server_fd = create_tcp_server(port);
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    SSL_CTX *ssl_ctx = create_ssl_context();
    configure_ssl_context(ssl_ctx);
    
    printf("HTTPS Server is running and waiting for connections...\n");

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("Error connection");
            continue;
        }

        printf("New client was connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        SSL *ssl = SSL_new(ssl_ctx);
        SSL_set_fd(ssl, client_socket);

        if (SSL_accept(ssl) < 0) {
            ERR_print_errors_fp(stderr);
        } else {
            handle_client(ssl);
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_socket);
    }

    SSL_CTX_free(ssl_ctx);
    return 0;
}

void handle_client(SSL *ssl) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    bytes_read = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("Error reading from client");
        return;
    }

    buffer[bytes_read] = '\0';
    printf("Receive from client: %s\n", buffer);

    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from secure server!\n";
    SSL_write(ssl, response, strlen(response));
}