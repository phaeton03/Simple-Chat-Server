#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    int status;
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "8088", &hints, &res);

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(socket_fd, res->ai_addr, res->ai_addrlen);
    listen(socket_fd, 2);

    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof their_addr;
    int connection_fd = accept(socket_fd, (struct sockaddr *) &their_addr, &addr_size);
    printf("Соединение установлено\n");

    while (1) {
        char msg[64] = {0};
        char ans[64] = {0};

        int bytes_received = recv(connection_fd, msg, sizeof(msg), 0);
        if (bytes_received <= 0) {
            break;
        }

        printf("Client: %s\n", msg);
        sprintf(ans, "Длина сообщения: %ld\n", strlen(msg));
        send(connection_fd, ans, strlen(ans), 0);
    }

    close(connection_fd);
    printf("Соединение закрыто\n");
    freeaddrinfo(res);
    close(socket_fd);

    return 0;
}
