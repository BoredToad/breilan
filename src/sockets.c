#include "sockets.h"

#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "breilan.h"

int create_socket(int port) {
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failed to make socket");
        return -1;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port ? port > 0 : DEFAULT_PORT),
    };

    if (bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind to port");
        return -2;
    }

    if (listen(fd, MAX_CLIENT_COUNT) < 0) {
        perror("Failed to listen on port");
        return -3;
    }

    return fd;
}

#define URLBUFFSIZE 1024

void* client_handler(void* args) {
    int client_fd = *((int*)args);
    char* recieved = malloc(BUFFERSIZE);

    ssize_t bytes_recieved = recv(client_fd, recieved, BUFFERSIZE, 0);

    char* content = "Hello from Server!";
    char* response = malloc(BUFFERSIZE);
    snprintf(response, BUFFERSIZE,
             "HTTP/1.1 200 OK\r\n"
             "Connection: close\r\n"
             "Content-Length: %lu\r\n"
             "Content-Type: text/plain\r\n"
             "\r\n"
             "%s",
             strlen(content), content);

    send(client_fd, response, BUFFERSIZE, 0);

    free(recieved);
    free(response);
    close(client_fd);
    free(args);
    return NULL;
}

void* client_handler2(void* args) {
    int x = *((int*)args);
    printf("x = %i\n", x);
    return NULL;
}

int handle_clients(int server_fd) {
    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int* client_fd = malloc(sizeof(int));

        if ((*client_fd = accept(server_fd, (struct sockaddr*)&client_addr,
                                 &client_addr_len)) < 0) {
            perror("failed to accept cliend");
            continue;
        }

        pthread_t thread_id;
        // thread gets ownership of args
        pthread_create(&thread_id, 0, client_handler, client_fd);
        pthread_detach(thread_id);
    }
}
