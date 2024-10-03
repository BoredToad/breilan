#define DEFAULT_PORT 80
// change the client count when ready for production
#define MAX_CLIENT_COUNT 10

int handle_clients(int server_fd);
int create_socket(int port);
void* client_handler(void* args);
