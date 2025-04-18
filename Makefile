# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 23:32:02 by caguillo          #+#    #+#              #
#    Updated: 2025/04/18 00:58:31 by caguillo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

SRCS		=	srcs/main.cpp \
				srcs/Server.cpp \
				srcs/Client.cpp \
				srcs/Register.cpp \
				srcs/Channel.cpp \
				srcs/Privmsg.cpp \
				srcs/Join.cpp \
				srcs/Who.cpp \
				srcs/Mode.cpp \
				srcs/Topic.cpp \
				srcs/Kick.cpp \
				srcs/Invite.cpp \
				srcs/Notice.cpp \
				srcs/Oper.cpp \
				srcs/Quit.cpp \
				srcs/Part.cpp \
				srcs/bot.cpp
				
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
