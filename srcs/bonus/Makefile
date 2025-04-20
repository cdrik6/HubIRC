# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 23:32:02 by caguillo          #+#    #+#              #
#    Updated: 2025/04/20 01:27:48 by caguillo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	bot

SRCS		=	main.cpp \
				Data.cpp \
				Game.cpp \
				Bot.cpp

CC			=	c++

CFLAGS		=	-Wall -Wextra -Werror -std=c++98

OBJS		=	$(SRCS:.cpp=.o)

$(NAME):	$(OBJS)			
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
			
all:		$(NAME)

re: 		fclean all
			
clean:			
			rm -f $(OBJS)

fclean: 	clean
			rm -f $(NAME)
			
.PHONY: 	all clean fclean re
