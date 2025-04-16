/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:50:24 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/16 03:35:11 by caguillo         ###   ########.fr       */
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

Channel::Channel(const Channel& other)
{
    *this = other;
}

Channel& Channel::operator=(const Channel& other)
{
    if (this == &other)
		return (*this);
    _name = other._name;
    _key = other._key;
    _topic = other._topic;
    _setby = other._setby;
    _setat = other._setat;
    _mode_i = other._mode_i;
	_mode_t = other._mode_t;
    _limit = other._limit;
    _chnlclts = other._chnlclts;
    _operators = other._operators;
	_invitees = other._invitees;
	return (*this);
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

std::vector<int> Channel::get_operators(void) const
{
	return (_operators);	
}

std::vector<int> Channel::get_invitees(void) const
{
	return (_invitees);
}

std::string Channel::get_key(void) const
{
	return (_key);
}

// std::vector<Client>& Channel::getref_chnlclts(void) 
// {
// 	return (_chnlclts);
// }

std::vector<int> Channel::get_chnlclts(void) const
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

void Channel::set_chnlclts(int clt_idx)
{
	_chnlclts.push_back(clt_idx);
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

// void Channel::add_operator(std::string nick)
// {
// 	_operators.push_back(nick);
// }
void Channel::add_operator(int clt_idx)
{
	_operators.push_back(clt_idx);
}
// Note
// An IRCop has full control over channels but does NOT get @ automatically.
// They must be explicitly given +o to appear as a channel operator.
// Even without @, they can still override channel protections.

void Channel::rem_operator(int clt_idx)
{
	for (int i = 0; i < _operators.size(); i++)
	{
		if (_operators.at(i) == clt_idx)			
		{
			_operators.erase(_operators.begin() + i);
			break;
		}			
	}	
}

bool Channel::is_operator(int clt_idx)
{
	for (int i = 0; i < _operators.size(); i++)
	{
		if (_operators.at(i) == clt_idx)
			return (true);					
	}
	return (false);
}

void Channel::add_invitee(int clt_idx)
{
	_invitees.push_back(clt_idx);
}

void Channel::rem_invitee(int clt_idx)
{
	for (int i = 0; i < _invitees.size(); i++)
	{
		if (_invitees.at(i) == clt_idx)
		{
			_invitees.erase(_invitees.begin() + i);
			break;
		}
	}
}

bool Channel::is_invitee(int clt_idx)
{
	for (int i = 0; i < _invitees.size(); i++)
	{
		if (_invitees.at(i) == clt_idx)
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