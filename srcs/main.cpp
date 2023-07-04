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

#include "../includes/Server.hpp"


int main (int argc, char  const**argv){
    if (argc != 3)
            throw std::logic_error("The program call : ./ircserv <port> <password>");
    int listener;     // Listening socket descriptor

    int newfd;        // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256];    // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    int fd_count = 0;
    int fd_size = 5;
    const char *port = argv[1];
    struct pollfd *pfds = static_cast<pollfd*>(malloc(sizeof *pfds * fd_size));

    // Set up and get a listening socket
    listener = get_listener_socket(port);

    if (listener == -1) {
        std::cerr << "error getting listening socket" << std::endl;
        std::exit(1);
    }

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

    fd_count = 1; // For the listener

    // Main loop
    for(;;) {
        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1) {
            std::cerr << "error poll" << std::endl;
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for(int i = 0; i < fd_count; i++) {

            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN) { // We got one!!
                if (pfds[i].fd == listener) {
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof(remoteaddr);
                    newfd = accept(listener, reinterpret_cast<struct sockaddr*>(&remoteaddr), reinterpret_cast<socklen_t*>(&addrlen));

                    if (newfd == -1) {
                        std::cerr << "error accept" << std::endl;
                    } else {
                        if (set_nonblocking(newfd) == -1) {
                            std::cerr << "set_nonblocking: " << strerror(errno) << std::endl;
                            close(newfd);
                            continue;
                        }
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
                        inet_ntop(remoteaddr.ss_family,
                                get_in_addr(reinterpret_cast<struct sockaddr*>(&remoteaddr)),
                                remoteIP, INET6_ADDRSTRLEN);

                        std::cout << "pollserver: new connection from " <<
                            remoteIP << " on socket " << newfd << std::endl;
                    }
                }else {
                    // If not the listener, we're just a regular client
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

                    int sender_fd = pfds[i].fd;

                    if (nbytes <= 0) {
                        // Got error or connection closed by client
                        if (nbytes == 0) {
                            // Connection closed
                            std::cout << "pollserver: socket " << sender_fd << " hung up" << std::endl;
                        } else {
                            std::cerr << ("recv") << std::endl;
}

                        close(pfds[i].fd); // Bye!

                        del_from_pfds(pfds, i, &fd_count);

                    } else {
                        // We got some good data from a client

                        for(int j = 0; j < fd_count; j++) {
                            // Send to everyone!
                            int dest_fd = pfds[j].fd;

                           // Except the listener and ourselves
                            if (dest_fd != listener && dest_fd != sender_fd) {
                                if (send(dest_fd, buf, nbytes, 0) == -1) {
                                    std::cerr << "send: " << strerror(errno) << std::endl;
                                }
                            }
                        }
                    }
                } // END handle data from client
            } // END got ready-to-read from poll()

            // Check if someone closed the socket
            if (pfds[i].revents & POLLRDHUP) {//POLLHUP: The file descriptor has been closed.
                std::cout << "pollserver: socket " << pfds[i].fd << " closed the connection" << std::endl;
                close(pfds[i].fd);
                del_from_pfds(pfds, i, &fd_count);
            }
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
    {

        int errorNumber = 0; // The error number you want to retrieve the message for
        char errorBuf[256]; // Buffer to store the error message

        // Retrieve the error message
        int result = atoi(strerror_r(errorNumber, errorBuf, sizeof(errorBuf)));

        if (result == 0) {
            // Successful retrieval of error message
            std::cout << "Error: " << errorBuf << std::endl;
        } else {
            // Error occurred while retrieving the error message
            std::cerr << "Failed to retrieve error message" << std::endl;
        }
    }
    
    return EXIT_SUCCESS;
}
