#include <stdio.h>
#include "socket.h"

int main() {
    int port = 8080; // Вибраний порт для сервера
    int server_fd = create_tcp_server(port);
    
    printf("HTTPS Server is running and waiting for connections...\n");
    
    // Тут будемо обробляти підключення клієнтів (пізніше)
    return 0;
}
