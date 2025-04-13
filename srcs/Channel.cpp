/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:50:24 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/13 01:46:50 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(): _name(""), _key(""), _topic(""),_setby(""), _setat(0)
{
	_mode_i = false;
    _mode_t = false;	
	_limit = -1;
}

Channel::~Channel()
{
	// std::cout << "Channel destructor called\n";
}

std::string Channel::get_name(void) const
{
	return (_name);	
}

std::string Channel::get_topic(void) const
{
	return (_topic);	
}

std::string Channel::get_setby(void) const
{
	return (_setby);	
}

time_t Channel::get_setat(void) const
{
	return (_setat);	
}

std::vector<std::string> Channel::get_operators(void) const
{
	return (_operators);	
}

std::vector<std::string> Channel::get_invitees(void) const
{
	return (_invitees);
}

std::string Channel::get_key(void) const
{
	return (_key);
}

std::vector<Client> Channel::get_chnlclts(void) const
{
	return (_chnlclts);
}

bool Channel::get_mode_i(void) const
{
	return (_mode_i);
}

bool Channel::get_mode_t(void) const
{
	return (_mode_t);
}

int Channel::get_limit(void) const
{	
	return (_limit);	
}

void Channel::set_name(std::string name)
{
	_name = name;	
}

void Channel::set_key(std::string key)
{
	_key = key;
}

void Channel::set_chnlclts(Client client)
{
	_chnlclts.push_back(client);
}

void Channel::set_mode_i(bool invite_only)
{
	_mode_i = invite_only;
}

void Channel::set_mode_t(bool restrict_topic)
{
	_mode_t = restrict_topic;
}

void Channel::set_limit(int limit)
{	
	_limit = limit;
}

void Channel::set_topic(std::string topic)
{
	_topic = topic;	
}

void Channel::set_setby(std::string nick)
{
	_setby = nick;
}

void Channel::set_setat(void)
{
	_setat = time(NULL);
}

void Channel::add_operator(std::string nick)
{
	_operators.push_back(nick);
}
// Note
// An IRCop has full control over channels but does NOT get @ automatically.
// They must be explicitly given +o to appear as a channel operator.
// Even without @, they can still override channel protections.

void Channel::rem_operator(std::string nick)
{
	for (int i = 0; i < _operators.size(); i++)
	{
		if (_operators.at(i) == nick)			
		{
			_operators.erase(_operators.begin() + i);
			break;
		}			
	}	
}

bool Channel::is_operator(std::string nick)
{
	for (int i = 0; i < _operators.size(); i++)
	{
		if (_operators.at(i) == nick)
			return (true);					
	}
	return (false);
}

void Channel::add_invitee(std::string nick)
{
	_invitees.push_back(nick);
}

void Channel::rem_invitee(std::string nick)
{
	for (int i = 0; i < _invitees.size(); i++)
	{
		if (_invitees.at(i) == nick)			
		{
			_invitees.erase(_invitees.begin() + i);
			break;
		}
	}
}

bool Channel::is_invitee(std::string nick)
{
	for (int i = 0; i < _invitees.size(); i++)
	{
		if (_invitees.at(i) == nick)
			return (true);					
	}
	return (false);
}

void Channel::rem_chnlclt(int chnlclt_idx)
{
	_chnlclts.erase(_chnlclts.begin() + chnlclt_idx);
}


/**** draft  */
// void Channel::rem_operator(std::string nick)
// {
// 	std::vector<std::string>::iterator it;
	
// 	for (it = _operators.begin(); it != _operators.end();)
// 	{
// 		if (*it == nick)			
// 		{
// 			it = _operators.erase(it);			
// 			break;
// 		}		
// 		else
// 			++it;
// 	}	
// }