#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];

    // Створення TCP-сокету
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Помилка при створенні сокету");
        exit(EXIT_FAILURE);
    }

    // Ініціалізація параметрів сервера
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Прив'язка сокету до адреси та порту
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Помилка при прив'язці сокету");
        exit(EXIT_FAILURE);
    }

    // Очікування підключення клієнта
    if (listen(server_socket, 5) == -1) {
        perror("Помилка при слуханні сокету");
        exit(EXIT_FAILURE);
    }

    printf("Сервер очікує підключення...\n");

    // Прийом та обробка підключення клієнта
    socklen_t client_address_len = sizeof(client_address);
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)) == -1) {
        perror("Помилка при прийомі підключення");
        exit(EXIT_FAILURE);
    }

    printf("Клієнт підключений\n");

    // Читання та відправлення даних
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received == -1) {
            perror("Помилка при отриманні даних");
            break;
        } else if (bytes_received == 0) {
            printf("Клієнт відключений\n");
            break;
        }

        printf("Отримано від клієнта: %s", buffer);

        // Додаткова обробка або логіка тут
// додати обробку помилок
        // Відправлення відповіді клієнту
        send(client_socket, buffer, bytes_received, 0);
    }

    // Закриття сокетів
    close(client_socket);
    close(server_socket);

    return 0;
}
