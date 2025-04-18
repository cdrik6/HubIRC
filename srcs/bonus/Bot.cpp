/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aoberon <aoberon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:29:56 by aoberon           #+#    #+#             */
/*   Updated: 2025/04/18 17:28:00 by aoberon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot( void )
{
	std::memset(&(this->_address), 0, sizeof(this->_address));
	this->_socketfd = 0;
	this->_botusername.clear();
	this->_botnickname.clear();

}

Bot::~Bot()
{
	if (this->_socketfd)
		close(this->_socketfd);
}

void	Bot::initialize_connection( std::string name, int port, std::string password )
{
	this->_botusername = name;
	this->_botnickname = name;

	this->_password = password;

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(port);
}

void	Bot::connect_to_server( void )
{
	this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!this->_socketfd)
        throw (std::runtime_error("socket: " + std::string(strerror(errno))));

	if (connect(this->_socketfd, (sockaddr*)&(this->_address), sizeof(this->_address)))
        throw (std::runtime_error("connect: " + std::string(strerror(errno))));

	std::string msg = "PASS " + this->_password + "\r\n" + "NICK " + this->_botnickname + "\r\n" + "USER " + this->_botusername + " 0 * :" + this->_botusername + "\r\n"; 

	if (send(this->_socketfd, msg.c_str(), msg.length(), MSG_NOSIGNAL) == -1)
	{
        throw (std::runtime_error("send: " + std::string(strerror(errno))));
	}
    std::cout << "Send: " + msg << std::endl;
}