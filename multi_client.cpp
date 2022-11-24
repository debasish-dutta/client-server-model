#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <ctime>
#include <cstdlib>

#define PORT 8080

using namespace std;

int main(int argc, char const* argv[]) {
    
    int client_socket_fd, valread;
    char client_read_buff[1024] = {0};

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = "127.0.0.1";
    server_addr.sin_port = htons(PORT);

    cout << "Client starts. Client pid is \n" << getpid();
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

    char server_ip[INET_ADDRSTRLEN] = "";
    inet_ntop(AF_INET, &server_addr.sin_addr, server_ip, INET6_ADDRSTRLEN);
    cout << "Client: connected to server(" << server_ip << ":" << ntohs(server_addr.sin_port) << ")" << endl;


    //Receiving from server

    valread = read(client_socket_fd, client_read_buff, 1024);
    cout << "Client: read: " << client_read_buff << endl;

    string recv_msg = string(client_read_buff);

    // sending to server

    string msg_str = "Hello from client.";
    char msg_char_array[msg_str.length() + 1];
    strcpy(msg_char_array, msg_str.c_str());
    send(client_socket_fd, &msg_char_array, strlen(msg_char_array), 0);
    cout << "Client: message sent: " << msg_char_array;




    close(client_socket_fd);
    return 0;

}
