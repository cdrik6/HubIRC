/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:29:53 by aoberon           #+#    #+#             */
/*   Updated: 2025/04/19 05:46:26 by caguillo         ###   ########.fr       */
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

# define OK 0
# define KO 1
# define PASSLEN 31
# define BUFFER_SIZE 1024

class Bot
{
	public:
		Bot();
		~Bot();
		void	initialize_connection(std::string name, int port, std::string password);
		void	connect_to_server(void);
		void	received_from_server(void);
		int	check_invite(std::string *channel_to_join);
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
};

std::vector<std::string> split(std::string str);

#endif