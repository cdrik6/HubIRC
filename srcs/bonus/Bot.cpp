/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:29:56 by aoberon           #+#    #+#             */
/*   Updated: 2025/04/19 23:06:34 by caguillo         ###   ########.fr       */
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
	reply("PASS " + this->_password + "\r\n" + "NICK " + this->_botnickname + "\r\n" + "USER " + this->_botusername + " 0 * :" + this->_botusername + "\r\n");
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
		_received = _received + std::string(buff);
		if (_received.find("\r\n") != std::string::npos)
		{
			_tab_recv.clear();
			_tab_recv = split(_received);	
			_received.clear();			
		}
	}
}

void Bot::check_invite(void)
{		
	std::string channel_to_join;
		
	for (int i = 0; i < _tab_recv.size(); i++)
	{			
		if (_tab_recv.at(i) == "INVITE")			
			if (i + 2 < _tab_recv.size())
				channel_to_join = _tab_recv.at(i + 2);
	}		
	if (channel_to_join != "")
	{				
		std::cout << channel_to_join << std::endl;		
		reply("JOIN " + channel_to_join + "\r\n"); 
	}
}

void Bot::check_join(void)
{		
	std::string channel_joined;
	
	for (int i = 0; i < _tab_recv.size(); i++)
	{			
		// if (_tab_recv.at(i).find(_botnickname) != std::string::npos)
		// if (i + 1 < _tab_recv.size())
		if (_tab_recv.at(i) == "JOIN")			
			if (i + 1 < _tab_recv.size())
				channel_joined = _tab_recv.at(i + 1);
	}		
	if (channel_joined != "")					
		_chan_game[channel_joined].set_step(0);
}

void Bot::check_privmsg(void)
{		
	std::string channel;
	std::string word;
	
	for (int i = 0; i < _tab_recv.size(); i++)
	{			
		if (_tab_recv.at(i) == "PRIVMSG")
		{
			if (i + 1 < _tab_recv.size())
				channel = _tab_recv.at(i + 1);
			if (i + 2 < _tab_recv.size())
				word = _tab_recv.at(i + 2);
		}							
	}		
	if (word == "cadavre exquis" && _chan_game[channel].get_step() == 0)
		_chan_game[channel].set_step(1);
	else if (word != "")
	{
		_chan_game[channel].set_word(word);
		_chan_game[channel].set_step(_chan_game[channel].get_step() + 1);
	}		
}

void Bot::reply(std::string msg)
{	
	if (send(this->_socketfd, msg.c_str(), msg.length(), MSG_NOSIGNAL) == -1)	
        throw (std::runtime_error("send: " + std::string(strerror(errno))));	
    std::cout << "Send:\n" + msg << std::endl;
}
