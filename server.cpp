#include <iostream>
#include <cstdio>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 3069
#define IP "172.0.0.1"
#define MAX_DATA 256

int process_client_connections(){    
    int server_socket_fd;
    
    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        perror("Socket Creation Failed");
        exit(EXIT_FAILURE);
    }
    
    int opt = 1;
    if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_socket; 
    serv_socket.sin_family = AF_INET;
    serv_socket.sin_addr.s_addr = INADDR_ANY; 
    serv_socket.sin_port = htons(PORT);
    
    if (bind(server_socket_fd, (struct sockaddr*)&serv_socket, sizeof(serv_socket)) < 0){
        perror("Binding Failed");
        exit(EXIT_FAILURE);
    } 

    if (listen(server_socket_fd, SOMAXCONN) < 0){ //maxes out connections to 128
        perror("Listening Failed");
        exit(EXIT_FAILURE);
    } 
    
    int client_socket_fd = -1; 
    struct sockaddr_in client_socket;
    int cs_size = sizeof(client_socket);
    
    if ((client_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_socket, (socklen_t*)&cs_size))< 0){
        perror("Accepting Connection Failed");
        exit(EXIT_FAILURE);
    }else std::cout << "Accepted Connection" << std::endl;
    
    return client_socket_fd;
}

int main(){    
    int client_socket_fd = process_client_connections();
    
    while(true){
        char send_buf[MAX_DATA];
        char read_buf[MAX_DATA];
        
        std::cout << "Your Message: ";
        std::cin.getline(send_buf, sizeof(send_buf));
        send(client_socket_fd, send_buf, sizeof(send_buf), 0);

        read(client_socket_fd, read_buf, sizeof(read_buf));
        std::cout << "Client Message: " << read_buf << std::endl; 
    }
    

    return 0;
}