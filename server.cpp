#include <iostream>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

using namespace std;

int main(int argc, char *argv[]) {

    int server_socket_fd, new_socket, valread;
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( PORT );
    int opt = 1;

    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Server: socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefullt attaching socket to 8080
    if (bind(server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Server: bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket_fd, 7) < 0) {
        perror("Server: listen");
        exit(EXIT_FAILURE);
    }
    cout << "Server: server is listening...\n\n";
    cout << "You can open one or more new ./client\n";

    int clients_ctr = 0;
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    char recv_buf[65536];
    memset(recv_buf, '\0', sizeof(recv_buf));

    if ((new_socket
         = accept(server_socket_fd, (struct sockaddr*)&server_address,
                  (socklen_t*)&length))
        < 0) {
        perror("Server: accept");
        exit(EXIT_FAILURE);
    }

    valread = read(new_socket, recv_buf, 1024);
    cout << recv_buf << endl;
    char *msg = "Hello from server!!";
    send(new_socket, msg, strlen(msg), 0);
    cout << "Server: sent msg to client.";

    close(new_socket);

    shutdown(server_socket_fd, SHUT_RDWR);

    return 0;
}