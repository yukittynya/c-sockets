#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUFFER 4096
#define LISTEN_BACKLOG 50
#define PORT 8080

void* handle_ftp(void* args) {
    int client_fd = *(int*)args;

    int size;
    if (read(client_fd, &size, sizeof(size)) < 0) {
        printf("ERROR: Reading file size\n");
        exit(1);
    }

    int filename_len;
    if (read(client_fd, &filename_len, sizeof(filename_len)) < 0) {
        printf("ERROR: Reading file name length");
        exit(1);
    }

    if (filename_len <= 0) {
        printf("ERROR: Filename length too short\n");
        exit(1);
    }

    char filename[filename_len + 1];

    if (read(client_fd, filename, filename_len) < 0) {
        printf("ERROR: Reading filename");
        exit(1);
    }

    filename[filename_len] = '\0';

    char filepath[256];

    struct stat st = {0};
    if (stat("outputs", &st) == -1) {
        mkdir("outputs", 0755);
    }
    snprintf(filepath, sizeof(filepath), "outputs/%s", filename); 

    float mb_size = (float) size / 1024 / 1024;
    printf("Downloading file: %s\nSize %.2f Mb\nOutput path: %s\n", filename, mb_size, filepath);

    FILE* fp;
    char buffer[MAX_BUFFER];
    int bytes_read = 0;
    int n = 0;

    fp = fopen(filepath, "wb");

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

    printf("SUCCESS: Downloaded %s\n", filename);
    
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
            char client_ipv4[INET_ADDRSTRLEN];

            inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ipv4, sizeof(client_ipv4));

            printf("Accepted client: %s\n", client_ipv4); 
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
