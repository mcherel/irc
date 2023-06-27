/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcherel- <mcherel-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 13:14:15 by mcherel-          #+#    #+#             */
/*   Updated: 2023/06/27 13:42:51 by mcherel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>
#include <stdexcept>

int main (int argc, char **argv){
    try{
        (void)argv;
        //std::cout << argc << std::endl;
        if (argc != 3)
            throw std::logic_error("The program call : ./ircserv <port> <password>");
    }
    catch( const std::exception & e ) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
