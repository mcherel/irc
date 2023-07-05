#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream> // Include the necessary header file for cout
# include <cstring> // Include the necessary header file for strerror
# include <cerrno> // Include the necessary header file for errno
# include <stdlib.h>
# include <stdexcept>
//#include <stdio.h>

//SOCKET RELATED LIB
# include <netinet/in.h> // Include the necessary header file for sockaddr_in and sockaddr_in6
# include <string>
# include <arpa/inet.h>
# include <poll.h>
# include <netdb.h> // Include the necessary header file for getaddrinfo, freeaddrinfo, gai_strerror
# include <sys/types.h> // Include the necessary header file for socket, setsockopt, bind, listen
# include <sys/socket.h> // Include the necessary header file for socket, setsockopt, bind, listen
# include <unistd.h> // Include the necessary header file for close
# include <fcntl.h> // Include the necessary header file for non-blocking mode
# include <map>
# include "Client.hpp"
class Server{

    private:
        int _server_socket;
        const std::string _port;
        const std::string _password;
        std::map<int, Client *> _clients;
        //commands
        //channels
        struct sockaddr_in _server;
        int get_socket();

    public:
        Server(
            const std::string & port, 
            const std::string & password
        );
        ~Server();

        void connect();
        //getters
        std::map<int, Client *> &get_clients();
        std::string get_input_from_client(int fd) const;
        std::string get_server_name() const;
        std::string get_port() const;
        struct sockaddr_in get_server();
        
        //setters
        void set_users(int, Client *);

        int search_client(std::string nickname);
        void clear_all();
};
//  Get sockaddr, IPv4 or IPv6:
void*   get_in_addr(struct sockaddr* sa);
//  Get listener socket
int get_listener_socket(const std::string port);
//  Add fd to pollfd structure used by poll()
void    add_to_pfds(pollfd* pfds[], int newfd, int* fd_count, int* fd_size);
//  Remove an index from the set
void    del_from_pfds(struct pollfd pfds[], int i, int *fd_count);
//  Set non blocking mode
int     set_nonblocking(int sockfd);
#endif

