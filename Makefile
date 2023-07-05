# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mcherel- <mcherel-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/27 12:54:02 by mcherel-          #+#    #+#              #
#    Updated: 2023/06/27 15:07:05 by mcherel-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

CXX		=	c++

src		=	main.cpp Server.cpp Client.cpp

OBJ		=	$(src:.cpp=.o)

FLAGS	=	-Wall -Wextra -Werror -std=c++98

all:		$(NAME)

%.o:		srcs/%.cpp
				$(CXX) $(FLAGS) -c $< -o $@

$(NAME):	$(OBJ)
				$(CXX) $(FLAGS) $(OBJ) -o $(NAME)
				@ echo "The program call : ./ircserv <port> <password>"
clean:
				rm -rf $(OBJ)
				
fclean:		clean
				rm -f $(NAME)

re:			fclean all

.PHONY:		all re clean fclean		