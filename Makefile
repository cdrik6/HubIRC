# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 23:32:02 by caguillo          #+#    #+#              #
#    Updated: 2025/04/28 11:43:10 by caguillo         ###   ########.fr        #
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
OBJS		=	$(SRCS:.cpp=.o)

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

$(NAME): $(OBJS)			
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	
%.o: %.cpp	
	$(CXX) $(CXXFLAGS) -c $< -o $@
			
all: $(NAME)

clean:
	rm -f $(OBJS)
	
fclean: clean
	rm -f $(NAME)
			
re: fclean all
			
.PHONY: all clean fclean re
