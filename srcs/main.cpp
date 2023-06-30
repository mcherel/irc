/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcherel- <mcherel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 13:14:15 by mcherel-          #+#    #+#             */
/*   Updated: 2023/06/30 13:41:13 by mcherel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include <stdexcept>

//SOCKET RELATED LIB
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080


int main (int argc, char  const**argv){
    try{
        (void)argv;
        //std::cout << argc << std::endl;
        if (argc != 3)
            throw std::logic_error("The program call : ./ircserv <port> <password>");
        int server_fd, new_socket, valread;
        //(void)new_socket;
        //(void)valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};
        std::string hello = "Hello from server";
        
        //Socket FD creation
        if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            throw std::logic_error("socket failed");

        //Attaching socket to the port 8080
        if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR \
            | SO_REUSEPORT, &opt, sizeof(opt))){
                throw std::logic_error("setsockopt");
            }
        address.sin_family = AF_INET;//IPv4
        address.sin_addr.s_addr = INADDR_ANY;//Binds Socket to all available interfaces
        address.sin_port = htons(PORT);

        if (bind(server_fd, (struct  sockaddr*)&address, sizeof(address)) < 0)
            throw std::logic_error("bind failed");
        
        if (listen(server_fd, 3) < 0)
            throw std::logic_error("listen");

        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
            throw std::logic_error("accept");
        
        valread = read(new_socket, buffer, 1024);
        std::cout << buffer << std::endl;
        send(new_socket, hello.c_str(), hello.size(), 0);
        std::cout << "Hello message sent" << std::endl;

        //https://www.geeksforgeeks.org/socket-programming-cc/
    }

    catch( const std::exception & e ) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
