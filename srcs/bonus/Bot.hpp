/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoberon <aoberon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:29:53 by aoberon           #+#    #+#             */
/*   Updated: 2025/04/18 17:13:06 by aoberon          ###   ########.fr       */
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

class Bot
{
	public:
		Bot();
		~Bot();

		void	initialize_connection( std::string name, int port, std::string password );
		void	connect_to_server( void );
		// void	routine( void );

	private:
		std::string	_password;

		int		_socketfd;
		struct sockaddr_in	_address;

		std::string	_botusername;
		std::string	_botnickname;

		// signal
};

#endif