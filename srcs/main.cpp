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

    Server	MY_SERVER( argv[1], argv[2] );
	MY_SERVER.connect();
    
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
