#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void chat(int client_socket) {
    char buffer[BUFFER_SIZE];
    int n;

    while (1) {
        // Очищуємо буфер
        memset(buffer, 0, sizeof(buffer));

        // Вводимо повідомлення від клієнта
        printf("Введіть повідомлення: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Відправка повідомлення на сервер
        write(client_socket, buffer, strlen(buffer));

        // Якщо введено "exit", завершимо програму
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Вихід з чату...\n");
            break;
        }

        // Очищуємо буфер для отримання відповіді
        memset(buffer, 0, sizeof(buffer));

        // Прийом відповіді від сервера
        n = read(client_socket, buffer, sizeof(buffer));

        // Відображення отриманого повідомлення
        printf("Відповідь від сервера: %s\n", buffer);
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // Створюємо TCP-сокет
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Не вдалось створити сокет");
        exit(EXIT_FAILURE);
    }

    // Налаштовуємо адресу сервера
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Підключення до сервера
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Підключення до сервера не вдалося");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Підключено до сервера!\n");

    // Запуск чату
    chat(client_socket);

    // Закриття сокета після завершення
    close(client_socket);
    return 0;
}
