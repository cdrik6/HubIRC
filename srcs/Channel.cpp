/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:50:24 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/03 21:14:37 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(): _name(""), _key(""), _topic("")
{}

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

std::vector<int> Channel::get_tab_clt_idx(void) const
{
	return (_tab_clt_idx);
}

void Channel::set_name(std::string name)
{
	_name = name;	
}

void Channel::set_key(std::string key)
{
	_key = key;
}

void Channel::set_tab_clt_idx(int clt_idx)
{
	_tab_clt_idx.push_back(clt_idx);
}

void Channel::add_operators(std::string nick)
{
	_operators.push_back(nick);
}
// Note
// An IRCop has full control over channels but does NOT get @ automatically.
// They must be explicitly given +o to appear as a channel operator.
// Even without @, they can still override channel protections.

void Channel::sub_operators(std::string nick)
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