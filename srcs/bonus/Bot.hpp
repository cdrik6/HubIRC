/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:29:53 by aoberon           #+#    #+#             */
/*   Updated: 2025/04/19 22:59:48 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <errno.h>
# include <vector>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <map>


# define OK 0
# define KO 1
# define PASSLEN 31
# define BUFFER_SIZE 1024

//# include "Game.hpp"
class Game;

class Bot
{
	public:
		Bot();
		~Bot();
		void	initialize_connection(std::string name, int port, std::string password);
		void	connect_to_server(void);
		void	received_from_server(void);
		void	check_invite(void);
		void	check_join(void);
		void	reply(std::string msg);
		void 	check_privmsg(void);
		// void	routine(void);
		
		static void	handle_signal(int signal);
		bool	get_signal(void) const;

	private:		
		int		_socketfd;
		struct sockaddr_in	_address;
		std::string	_password;
		std::string	_botusername;
		std::string	_botnickname;
		static bool _signal;
		std::string	_received;
		std::vector<std::string> _channel_joined;
		std::vector<std::string> _tab_recv;
		std::map<std::string, Game> _chan_game;
};

std::vector<std::string> split(std::string str);

#endif