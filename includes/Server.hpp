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


#include <iostream> // Include the necessary header file for cout
#include <cstring> // Include the necessary header file for strerror
#include <cerrno> // Include the necessary header file for errno
#include <stdlib.h>
#include <stdexcept>
//#include <stdio.h>

//SOCKET RELATED LIB
#include <netinet/in.h> // Include the necessary header file for sockaddr_in and sockaddr_in6
#include <string>
#include <arpa/inet.h>
#include <poll.h>
#include <netdb.h> // Include the necessary header file for getaddrinfo, freeaddrinfo, gai_strerror
#include <sys/types.h> // Include the necessary header file for socket, setsockopt, bind, listen
#include <sys/socket.h> // Include the necessary header file for socket, setsockopt, bind, listen
#include <unistd.h> // Include the necessary header file for close
#include <fcntl.h> // Include the necessary header file for non-blocking mode


//  Get sockaddr, IPv4 or IPv6:
void*   get_in_addr(struct sockaddr* sa);
//  Get listener socket
int get_listener_socket(const char* port);
//  Add fd to pollfd structure used by poll()
void    add_to_pfds(pollfd* pfds[], int newfd, int* fd_count, int* fd_size);
//  Remove an index from the set
void    del_from_pfds(struct pollfd pfds[], int i, int *fd_count);
//  Set non blocking mode
int     set_nonblocking(int sockfd);