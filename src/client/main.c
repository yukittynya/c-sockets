#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER 128
#define PORT 8080

void handle_chat(int socket_fd) {
    char buffer[MAX_BUFFER];
    int i = 0;
    
    for (;;) {
        bzero(buffer, MAX_BUFFER);

        printf("Enter message: ");
        i = 0;

        while ((buffer[i++] = getchar()) != '\n');

        write(socket_fd, buffer, sizeof(buffer));
        bzero(buffer, sizeof(buffer));

        read(socket_fd, buffer, sizeof(buffer));

        printf("\nServer: %s\n", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Client exit\n");
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    struct sockaddr_in client_addr;
    int socket_fd;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERROR: Creating socket failed\n");
        exit(1);
    } else {
        printf("Created socket\n");
    }

    bzero(&client_addr, sizeof(client_addr));

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(socket_fd, (struct sockaddr*) &client_addr, sizeof(client_addr)) != 0) {
        printf("ERROR: Unable to connect\n");
        exit(1);
    } else {
        printf("Connected\n");
    }

    handle_chat(socket_fd);

    close(socket_fd);
}
