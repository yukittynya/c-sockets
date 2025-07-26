#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER 128
#define LISTEN_BACKLOG 50
#define PORT 8080

void handle_chat(int client_fd) {
    char buffer[MAX_BUFFER];
    int i = 0;
    
    for (;;) {
        bzero(buffer, MAX_BUFFER);

        read(client_fd, buffer, sizeof(buffer));
        printf("\nUser: %s\nEnter response: ", buffer);

        bzero(buffer, MAX_BUFFER);

        i = 0;
        
        while ((buffer[i++] = getchar()) != '\n'); 

        write(client_fd, buffer, sizeof(buffer));

        if (strncmp("exit", buffer, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        }

    }
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    int socket_fd;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("ERROR: Creating socket failed");
        exit(1);
    } else {
        printf("Created socket");
    }

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        printf("ERROR: Unable to bind\n");
        exit(1);
    } else {
        printf("Binded socket at port: %d\n", PORT);
    }

    if (listen(socket_fd, LISTEN_BACKLOG) < 0) {
        printf("ERROR: Unable to listen\n");
        exit(1);
    } else {
        printf("Socket is listening\n");
    }
    
    struct sockaddr_in client_addr;
    int client_fd;
    socklen_t client_size = sizeof(client_addr);

    if ((client_fd = accept(socket_fd, (struct sockaddr*) &client_addr, &client_size)) < 0) {
        printf("ERROR: Unable to accept client\n");
        exit(1);
    } else {
        printf("Accepted client\n");
    }

    handle_chat(client_fd);

    close(socket_fd);
}
