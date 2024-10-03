#include <unistd.h>

#include "sockets.h"

int main() {
    int server_fd;
    if ((server_fd = create_socket(0)) < 0) {
        return server_fd;
    }

    int error = handle_clients(server_fd);

    close(server_fd);
    if (error != 0) return error;
    return 0;
}
