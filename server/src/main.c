#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void handle_client(int client_socket);

int main() {
    int port = 8080;
    int server_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    
    // Створюємо TCP-сокет
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Не вдалось створити сокет");
        exit(EXIT_FAILURE);
    }

    // Налаштовуємо адресу сервера
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Прив'язуємо сокет до адреси та порту
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Помилка прив'язки сокета");
        exit(EXIT_FAILURE);
    }

    // Починаємо слухати вхідні підключення
    if (listen(server_fd, 1) < 0) {
        perror("Помилка прослуховування");
        exit(EXIT_FAILURE);
    }

    printf("Сервер працює і очікує підключень...\n");

    while (1) {
        // Приймаємо підключення від клієнта
        int client_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("Помилка підключення");
            continue;
        }

        printf("Підключився новий клієнт: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Обробка клієнта (чат)
        handle_client(client_socket);

        // Закриваємо підключення після завершення
        close(client_socket);
    }

    close(server_fd);
    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while (1) {
        // Очищуємо буфер
        memset(buffer, 0, sizeof(buffer));

        // Читання повідомлення від клієнта
        bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Помилка читання від клієнта");
            return;
        }

        // Завершуємо рядок
        buffer[bytes_read] = '\0';
        printf("Отримано від клієнта: %s\n", buffer);

        // Якщо клієнт відправив "exit", завершити обробку
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Клієнт завершив сеанс.\n");
            break;
        }

        // Відповідь серверу
        const char *response = "Привіт від сервера!\n";
        write(client_socket, response, strlen(response));
    }
}
