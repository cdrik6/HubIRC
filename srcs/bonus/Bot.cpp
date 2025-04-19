/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandm <alexandm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:29:56 by aoberon           #+#    #+#             */
/*   Updated: 2025/04/19 21:45:05 by alexandm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(void)
{
	std::memset(&(this->_address), 0, sizeof(this->_address));
	this->_socketfd = -1;
	this->_botusername.clear();
	this->_botnickname.clear();
	this->_received.clear();
}

Bot::~Bot()
{
	if (this->_socketfd != -1)
		close(this->_socketfd);
}

bool	Bot::_signal = false;

void	Bot::handle_signal(int signal)
{
    std::cout << "\nSignal " << signal << " received" << std::endl;
    _signal = true;
}

bool	Bot::get_signal(void) const
{
	return (this->_signal);
}

void	Bot::initialize_connection(std::string name, int port, std::string password)
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
	if (this->_socketfd == -1)
        throw (std::runtime_error("socket: " + std::string(strerror(errno))));
	if (connect(this->_socketfd, (sockaddr*)&(this->_address), sizeof(this->_address)) == -1)
        throw (std::runtime_error("connect: " + std::string(strerror(errno))));

	std::string msg = "PASS " + this->_password + "\r\n" + "NICK " + this->_botnickname + "\r\n" + "USER " + this->_botusername + " 0 * :" + this->_botusername + "\r\n"; 

	if (send(this->_socketfd, msg.c_str(), msg.length(), MSG_NOSIGNAL) == -1)	
        throw (std::runtime_error("send: " + std::string(strerror(errno))));
	
    std::cout << "Send:\n" + msg << std::endl;
	
}

void	Bot::received_from_server()
{
	char buff[BUFFER_SIZE + 1] = {0};
	int nbytes = recv(_socketfd, buff, BUFFER_SIZE, 0);	
	
	std::cout << "Received:\n[" << buff << "]\n" << std::endl;	
	if (nbytes <= 0) // closed or issues
	{
		if (nbytes == 0)						
			std::cout << "Server closed the connection" << std::endl;
		if (nbytes == -1)			
			throw (std::runtime_error("recv: " + std::string(strerror(errno))));			
	}
	else // got some data from server
	{
		std::string channel_to_join;
		_received = _received + std::string(buff);
		
		if (check_invite(&channel_to_join) == OK)
		{
			if (channel_to_join != "")
			{				
				std::cout << channel_to_join << std::endl;
				// join /********* a continuer ici */
			}				
		}
		// _received.clear();			
	}
}

int Bot::check_invite(std::string *channel)
{	
	std::vector<std::string> tab_recv;
		
	if (_received.find("\r\n") != std::string::npos)
	{		
		tab_recv = split(_received);
		if (tab_recv.size() == 0)
			return (std::cout << "tab empty" << std::endl, KO);
		for (int i = 0; i < tab_recv.size(); i++)
		{			
			if (tab_recv.at(i) == "INVITE")	
			{
				if (i + 2 < tab_recv.size())
					*channel = tab_recv.at(i + 2);
				else
					*channel = "";
				return (OK);
			}
		}		
		return (KO);
	}	
	return (KO);
}

void Bot::select_initialisation()
{
	FD_ZERO(&_currentfds);
	FD_SET(this->_socketfd, &_currentfds);
}

bool Bot::fd_ready_for_recv()
{
	int ret;

	_readfds = _currentfds;
	ret = select(_socketfd + 1, &_readfds, NULL, NULL, NULL);
	if (_signal == true)
		return (false);
	if (ret == -1)
			throw (std::runtime_error("select: " + std::string(strerror(errno))));
	if (FD_ISSET(this->_socketfd, &_readfds))
		return (true);
	return (false);
}