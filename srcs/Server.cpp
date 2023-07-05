
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcherel- <mcherel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 13:14:15 by mcherel-          #+#    #+#             */
/*   Updated: 2023/06/30 13:41:13 by mcherel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server( 
        const std::string & port, 
        const std::string & password
    ):
        _port(port),
        _password(password){
    //commands initialisation part
}

Server::~Server(){
    //clearing commands part
}

void Server::connect(){
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
    const std::string port = this->get_port();
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
}

std::string Server::get_port() const {
    return this->_port;
}

int Server::get_socket(){
    return 1;//to fill
}
// Get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(reinterpret_cast<struct sockaddr_in*>(sa)->sin_addr);
    }

    return &(reinterpret_cast<struct sockaddr_in6*>(sa)->sin6_addr);
}



int get_listener_socket(const std::string port)
{
    int listener; // Listening socket descriptor
    int yes = 1; // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof(hints)); // Clear the hints structure
    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE; // For wildcard IP address
    /* The getaddrinfo() function is used to retrieve address information 
    for a given host and service. In this case, 
    NULL is passed as the host parameter, which indicates that 
    the local host address will be used. 
    PORT represents the port number or service name. */
    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &ai)) != 0) {
        std::cerr << "selectserver: " << gai_strerror(rv) << std::endl; // Print the error message
        std::exit(1); // Exit the program with an error status
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol); // Create the socket
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)); // Set the socket option

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) { // Bind the socket
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1) { // Start listening on the socket
        return -1;
    }

    return listener; // Return the listening socket descriptor
}

/* The poll() function takes an array of pollfd structures 
and monitors the specified file descriptors for the specified event.
pollfd is a structure used in the poll() function for monitoring multiple file descriptors for events. 
It is defined in the <poll.h> header file. The pollfd structure has the following members:

    * fd: This member represents the file descriptor being monitored.
    * events: This member specifies the events to monitor for the file descriptor. It is a bitmask that can include the following values:
        POLLIN: Data is available for reading.
        POLLOUT: The file descriptor is ready for writing.
        POLLERR: An error has occurred on the file descriptor.
        POLLHUP: The file descriptor has been closed.
        POLLNVAL: The file descriptor is not open.
    * revents: This member is set by the poll() function and indicates the events that have occurred on the file descriptor. 
    It is a bitmask that can include the same values as events.*/

//Adding fd to pollfd structure used by poll()
void add_to_pfds(pollfd* pfds[], int newfd, int* fd_count, int* fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        // Reallocate memory for the pfds array, using reinterpret_cast for the type cast
        *pfds = reinterpret_cast<pollfd*>(realloc(*pfds, sizeof(**pfds) * (*fd_size)));
    }

    // Assign the new file descriptor and events to the pfds array
    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    // Set the socket to non-blocking mode
    //fcntl(newfd, F_SETFL, O_NONBLOCK);

    // Increment the fd_count
    (*fd_count)++;
}

/* The fcntl() function is used to set the socket file descriptors to
 non-blocking mode. Additionally, the POLLRDHUP flag is added to 
 the events field of the pollfd structure to detect 
 if a client closed the connection. */

// Remove an index from the set
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    // Copy the one from the end over this one
    pfds[i] = pfds[*fd_count-1];

    (*fd_count)--;
}
/*  It uses fcntl to get the current socket flags 
 and then sets the O_NONBLOCK flag using bitwise OR (|)
 with the existing flags. */

//Set non blocking mode
int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        char errorBuf[256];
        strerror_r(errno, errorBuf, sizeof(errorBuf));
        std::cerr << "fcntl F_GETFL: " << errorBuf << std::endl;
        return -1;
    }

    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        char errorBuf[256];
        strerror_r(errno, errorBuf, sizeof(errorBuf));
        std::cerr << "fcntl F_SETFL O_NONBLOCK: " << errorBuf << std::endl;
        return -1;
    }
    return 0;
}

