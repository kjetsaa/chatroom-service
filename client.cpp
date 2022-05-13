#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_DATA 256

void display_title(){ 
    std::cout << "\n========= CHAT ROOM CLIENT =========\n";
    std::cout << " Command Lists and Format:\n";
    std::cout << " CREATE <name>\n";
    std::cout << " JOIN <name>\n";
    std::cout << " DELETE <name>\n";
    std::cout << " LIST\n";
    std::cout << "=====================================\n"; 
}

//Handles posix client tcp channel
int process_server_connection(const char* IP, const char* PORT){
    int client_socket_fd;

    // Initializing client socket file descriptor
     if ((client_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        perror("Socket Creation Failed");
        exit(EXIT_FAILURE);
    } 

    //Connect to given server
    struct sockaddr_in server_socket_fd;
    server_socket_fd.sin_family = AF_INET;
    server_socket_fd.sin_port = htons(atoi(PORT));
    inet_pton(AF_INET, PORT, &(server_socket_fd.sin_addr));

    if (connect(client_socket_fd, (struct sockaddr*)&server_socket_fd, sizeof(server_socket_fd)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    return client_socket_fd;
}


int main(int argc, char const* arv[]){

    if (argc != 3){
        std::cout << "Not enough arguments" << std::endl;
        return -1;
    }

    const char* ip = arv[1];
    const char* port = arv[2];
    
    int client_socket_fd = process_server_connection(ip, port);

    display_title();

   while(true){
        char send_buf[MAX_DATA];
        char read_buf[MAX_DATA];

        read(client_socket_fd, read_buf, sizeof(read_buf));
        std::cout << "Server Message: " << read_buf << std::endl; 
        
        std::cout << "Your Message: ";
        std::cin.getline(send_buf, sizeof(send_buf));
        send(client_socket_fd, send_buf, sizeof(send_buf), 0);      
    }

    return 0;
}