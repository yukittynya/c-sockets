#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LISTEN_BACKLOG 50

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    int socket_fd;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERROR: Creating socket failed");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        printf("ERROR: Unable to bind");
        exit(1);
    }

    if (listen(socket_fd, LISTEN_BACKLOG) < 0) {
        printf("ERROR: Unable to listen");
        exit(1);
    }

    printf("testestse");
}
