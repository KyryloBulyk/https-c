#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket.h"

#define BUFFER_SIZE 1024

void handle_client(int client_socket);

int main() {
    int port = 8080;
    int server_fd = create_tcp_server(port);
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    
    printf("HTTP Server is running and waiting for connections...\n");

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("Error connection");
            continue;
        }

        printf("New client was connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Обробляємо запит клієнта
        handle_client(client_socket);

        // Закриваємо підключення після обробки
        close(client_socket);
    }

    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Читання даних від клієнта через звичайне TCP-з'єднання
    bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Error reading from client");
        return;
    }

    // Завершуємо рядок
    buffer[bytes_read] = '\0';
    printf("Received from client: %s\n", buffer);

    // Підготовка HTTP-відповіді
    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from server!\n";
    
    // Відправка відповіді клієнту
    write(client_socket, response, strlen(response));
}
