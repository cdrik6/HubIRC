# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/25 23:32:02 by caguillo          #+#    #+#              #
#    Updated: 2025/04/27 22:54:06 by caguillo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	bot
SRCS		=	main.cpp \
				Data.cpp \
				Game.cpp \
				Bot.cpp				
OBJS		=	$(SRCS:.cpp=.o)

CXX			=	g++
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
			
.PHONY: all clean fclean re $(NAME)
