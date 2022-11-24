#include <iostream>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <ctime>
#include <cstdlib>

#define PORT 8080

using namespace std;

int main(int argc, char *argv[]) {

    int server_socket_fd, new_socket, valread;
    vector<int> client_sockets;
    vector<string> client_ips;
    vector<int> client_ports;
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

    int in_client_no = 1;
    while(in_client_no == 1) {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

    if((new_socket = accept(server_socket_fd, (struct sockaddr *)&client_addr, 
                        (socklen_t*)&length))<0) {
                            perror("Server: accept");
                            exit(EXIT_FAILURE);
                        }
    clients_ctr++;
    cout << "You have connected with " << clients_ctr << "client(s) now." << endl;

    char client_ip[INET6_ADDRSTRLEN] = "";
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    cout << "Server: new client accepted, Client ip & port: " << client_ip << ":" << ntohs(client_addr.sin_port) << endl;

    // store multiple clients
    client_sockets.push_back(new_socket);
    client_ips.push_back(client_ip);
    client_ports.push_back(ntohs(client_addr.sin_port));

    cout <<"Current clients connected: " << client_sockets.size() << endl;

    cout << "Do you want to add more clients? ('1' for yes and '0' for No)" << endl;
    cin >> in_client_no;

    if (in_client_no == 1) {
        cout << "OK. Open more ./clients in other windows." << endl;
    } else if (in_client_no != 0) {
        cout << "Unrecognized input has been considered as 1. You can create one more client.\n" << endl;
        in_client_no = 1;
        }

    }

    cout << "Clients creations finished. There are total " << client_sockets.size() << "connected clients." << endl;

    // Broadcasting to all clients

    for(int i=0; i<client_sockets.size(); i++) {
        //Sending
        const char *msg = "Hello from server.";
        send(client_sockets[i], msg, strlen(msg), 0);
        cout << "Server: sent to client(" << client_ips[i].c_str() << ":" << client_ports[i] << ")" << endl;

        //Receiving

        while(recv(client_sockets[i], recv_buf, sizeof(recv_buf), 0) > 0) {
            cout << "Server: received from client(" << client_ips[i].c_str() << ":" << client_ports[i] << "): " << recv_buf << endl;

        }
    }

    cout << "Server: server stopped." << endl;

    close(new_socket);

    shutdown(server_socket_fd, SHUT_RDWR);

    return 0;
}