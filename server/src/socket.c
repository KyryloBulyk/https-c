#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "socket.h"

int create_tcp_server(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Create of Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Unable to connect to the socket");
        exit(EXIT_FAILURE);
    }

    // Setting parameters of socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Error in setting parameter of socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Socket binding to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error in socker binding");
        exit(EXIT_FAILURE);
    }

    // Start of listening on socket
    if (listen(server_fd, 3) < 0) {
        perror("Error in listeting");
        exit(EXIT_FAILURE);
    }

    printf("Server was started on port %d\n", port);
    return server_fd;
}

SSL_CTX *create_ssl_context() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_ssl_algorithms();
    method = SSLv23_server_method();
    ctx = SSL_CTX_new(method);

    if (!ctx) {
        perror("Cannot to create SSL contex");
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_ssl_context(SSL_CTX *ctx) {
    if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}
