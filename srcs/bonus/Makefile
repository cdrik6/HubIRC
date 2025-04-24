# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 23:32:02 by caguillo          #+#    #+#              #
#    Updated: 2025/04/24 17:03:33 by caguillo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	bot

SRCS		=	main.cpp \
				Data.cpp \
				Game.cpp \
				Bot.cpp

CXX			=	g++

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g

OBJS		=	$(SRCS:.cpp=.o)

%.o:%.cpp		
			$(CXX) $(CXXFLAGS) -c $< -o $(<:.cpp=.o)

$(NAME):	$(OBJS)			
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
			
all:		$(NAME)

re: 		fclean all
			
clean:			
			rm -f $(OBJS)

fclean: 	clean
			rm -f $(NAME)
			
.PHONY: 	all clean fclean re
