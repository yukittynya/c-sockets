#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER 4096 
#define PORT 8080

void handle_ftp(int socket_fd) {
    FILE* fp = fopen("test.png", "rb");

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (write(socket_fd, &size, sizeof(size)) < 0) {
        printf("ERROR: Unable to send size\n");
        exit(1);
    }

    char buffer[MAX_BUFFER]; 
    int bytes_read = 0;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        if (write(socket_fd, buffer, sizeof(buffer)) < 0) {
            printf("ERROR: Unable to send file");
            exit(1);
        } 
    }

    fclose(fp);
}

void* client_thread() {
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
    client_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(socket_fd, (struct sockaddr*) &client_addr, sizeof(client_addr)) != 0) {
        printf("ERROR: Unable to connect\n");
        exit(1);
    } else {
        printf("Connected\n");
    }

    handle_ftp(socket_fd);

    close(socket_fd);

    pthread_exit(NULL);

    return 0;
}

int main(int argc, char *argv[]) {
    pthread_t tid;

    pthread_create(&tid, NULL, client_thread(), NULL);

    pthread_join(tid, NULL);
}
