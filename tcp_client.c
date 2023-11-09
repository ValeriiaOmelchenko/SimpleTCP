#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Створення TCP-сокету
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Помилка при створенні сокету");
        exit(EXIT_FAILURE);
    }

    // Ініціалізація параметрів сервера
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    // Встановлення підключення до сервера
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Помилка при підключенні до сервера");
        exit(EXIT_FAILURE);
    }

    printf("Підключено до сервера\n");

    // Відправлення та читання даних
    while (1) {
        printf("Введіть повідомлення для сервера: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Відправлення даних серверу
        send(client_socket, buffer, strlen(buffer), 0);

        // Очищення буфера
        memset(buffer, 0, sizeof(buffer));

        // Читання відповіді від сервера
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received == -1) {
            perror("Помилка при отриманні даних");
            break;
        } else if (bytes_received == 0) {
            printf("Сервер відключений\n");
            break;
        }

        printf("Отримано від сервера: %s", buffer);
    }

    // Закриття сокету
    close(client_socket);

    return 0;
}
