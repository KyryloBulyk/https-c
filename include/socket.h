#ifndef SOCKET_H
#define SOCKET_H

#include <openssl/ssl.h>
#include <openssl/err.h>

int create_tcp_server(int port);
SSL_CTX *create_ssl_context();
void configure_ssl_context(SSL_CTX *ctx);

#endif // SOCKET_H
