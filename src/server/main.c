#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER 4096
#define LISTEN_BACKLOG 50
#define PORT 8080

void* handle_ftp(void* args) {
    int client_fd = *(int*)args;
    FILE* fp;
    int n = 0;

    fp = fopen("received.png", "wb");

    int size;
    read(client_fd, &size, sizeof(size));

    char buffer[MAX_BUFFER];
    int bytes_read = 0;

    while (bytes_read < size) {
        n = read(client_fd, buffer, sizeof(buffer)); 

        if (n < 0) {
            printf("ERROR: recv() failed\n");
            exit(1);
        }

        if (n == 0) {
            printf("Client disconnected\n");
            break;
        }

        fwrite(buffer, 1, n, fp);
        bytes_read += n;
    }

    
    fclose(fp);

    pthread_exit(NULL);

    return 0;
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

    pthread_t tid[10];
    int i = 0;

    struct sockaddr_in client_addr;
    int client_fd;
    socklen_t client_size;

    while (1) {
        client_size = sizeof(client_addr);

        if ((client_fd = accept(socket_fd, (struct sockaddr*) &client_addr, &client_size)) < 0) {
            printf("ERROR: Unable to accept client\n");
            exit(1);
        } else {
            printf("Accepted client\n");
            pthread_create(&tid[i++], NULL, handle_ftp, &client_fd);
        }

        if (i >= 10) {
            i = 0;

            while (i < 10) {
                pthread_join(tid[i++], NULL);
            }
        }
    }

    close(socket_fd);
}
