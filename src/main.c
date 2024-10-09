#include <stdio.h>
#include "socket.h"

int main() {
    int port = 8080;
    int server_fd = create_tcp_server(port);
    
    printf("HTTPS Server is running and waiting for connections...\n");
    
    return 0;
}
