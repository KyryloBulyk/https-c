#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket.h"

int create_tcp_server(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Creating TCP-socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    // Set parametes for socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Error in setting socket options");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Binding socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error in socket binding");
        exit(EXIT_FAILURE);
    }

    // Starting listening on socket
    if (listen(server_fd, 3) < 0) {
        perror("Error in listening");
        exit(EXIT_FAILURE);
    }

    printf("Server was started on port %d\n", port);
    return server_fd;
}
