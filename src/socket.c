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

    // Створення сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Не вдалось створити сокет");
        exit(EXIT_FAILURE);
    }

    // Налаштування параметрів сокета
    // Заміна старого рядка setsockopt
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Помилка налаштування сокета");
        exit(EXIT_FAILURE);
    }


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Прив'язка сокета до порту
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Помилка прив'язки сокета");
        exit(EXIT_FAILURE);
    }

    // Початок прослуховування на сокеті
    if (listen(server_fd, 3) < 0) {
        perror("Помилка прослуховування");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущено на порту %d\n", port);
    return server_fd;
}
