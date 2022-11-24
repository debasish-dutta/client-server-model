#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

using namespace std;

int main(int argc, char const* argv[]) {
    
    int client_socket_fd, valread;
    char client_read_buff[1024] = {0};

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = "127.0.0.1";
    server_addr.sin_port = htons(PORT);

    //Creating socket
    if((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Client: Socket creation error!!" << endl;
        return -1;
    }

    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        cout << "Client: Invalid address.";
        return -1;
    }

    if(connect(client_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        cout << "Client: COnnection failed.";
        return -1;
    }

    char *msg = "Hello from client!!";
    send(client_socket_fd, msg, strlen(msg), 0);
    valread = read(client_socket_fd, client_read_buff, 1024);
    cout << client_read_buff << endl;


    close(client_socket_fd);
    return 0;

}
