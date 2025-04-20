# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 23:32:02 by caguillo          #+#    #+#              #
#    Updated: 2025/04/20 01:32:26 by caguillo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

SRCS		=	srcs/main.cpp \
				srcs/Server.cpp \
				srcs/Client.cpp \
				srcs/Register.cpp \
				srcs/Channel.cpp \
				srcs/privmsg.cpp \
				srcs/join.cpp \
				srcs/who.cpp \
				srcs/mode.cpp \
				srcs/topic.cpp \
				srcs/kick.cpp \
				srcs/invite.cpp \
				srcs/notice.cpp \
				srcs/oper.cpp \
				srcs/quit.cpp \
				srcs/part.cpp \
				srcs/iambot.cpp
				
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
