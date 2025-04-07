/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:50:24 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/07 03:53:15 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(): _name(""), _key(""), _topic("")
{
	_mode_i = false;
    _mode_t = false;
	_mode_k = false;
	_mode_o = false;
	_mode_l = false;
	_limit = -1;
}

Channel::~Channel()
{}

std::string Channel::get_name(void) const
{
	return (_name);	
}

std::string Channel::get_topic(void) const
{
	return (_topic);	
}

std::vector<std::string> Channel::get_operators(void) const
{
	return (_operators);	
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

bool Channel::get_mode_k(void) const
{
	return (_mode_k);	
}

bool Channel::get_mode_o(void) const
{
	return (_mode_o);
}

bool Channel::get_mode_l(void) const
{
	return (_mode_l);
}

int Channel::get_limit(void) const
{
	if (get_mode_l())
		return (_limit);
	return (-1);	
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

void Channel::set_limit(int limit)
{
	if (get_mode_l())
		_limit = limit;
	else 	
		_limit = -1;
}

void Channel::add_operator(std::string nick)
{
	_operators.push_back(nick);
}
// Note
// An IRCop has full control over channels but does NOT get @ automatically.
// They must be explicitly given +o to appear as a channel operator.
// Even without @, they can still override channel protections.

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

void Channel::rem_client(int idx)
{
	_chnlclts.erase(_chnlclts.begin() + idx);
}
