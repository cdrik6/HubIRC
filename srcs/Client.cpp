/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:15:28 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/15 03:09:25 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() :
_clt_skt(-1),
_registered(false),
_pwd_ok(false),
_nickname("*"),
_username(""),
_hostname(""),
_realname(""),
_msg(""),
_is_bot(false),
_bot_step(0)
{}

Client::~Client()
{
    // std::cout << "Client destructor called\n";
}

Client::Client(const Client& other)
{
    *this = other;
}

Client& Client::operator=(const Client& other)
{
    if (this == &other)
		return (*this);
    _clt_skt = other._clt_skt;
    _registered = other._registered;
    _pwd_ok = other._pwd_ok;    
    _nickname = other._nickname;
    _username = other._username;
    _hostname = other._hostname;    
    _realname = other._realname;
    _msg = other._msg;
    _is_bot = other._is_bot;
    _bot_step = other._bot_step;    
    _words = other._words;
	return (*this);
}

int Client::get_clt_skt(void) const
{
    return(_clt_skt);
}

std::string Client::get_msg(void) const
{
    return(_msg);
}

std::string Client::get_nickname(void) const
{
    return(_nickname);
}

std::string Client::get_hostname(void) const
{
    return(_hostname);
}

std::string Client::get_username(void) const
{
    return(_username);
}

std::string Client::get_realname(void) const
{
    return(_realname);
}

bool Client::get_pwd_ok(void) const
{
    return (_pwd_ok);
}

bool Client::get_registered(void) const
{
    return (_registered);
}

bool Client::get_is_bot(void) const
{
    return (_is_bot);
}

int Client::get_bot_step(void) const
{
    return (_bot_step);
}

std::vector<std::string> Client::get_words(void) const
{
    return (_words);
}

void Client::set_clt_skt(int clt_skt)
{
    _clt_skt = clt_skt;
}

void Client::set_nickname(std::string nickname)
{
    _nickname = nickname;
}

void Client::set_username(std::string username)
{
    _username = username;
}

void Client::set_realname(std::string realname)
{
    _realname = realname;
}

void Client::set_pwd_ok(bool pwd_ok)
{
    _pwd_ok = pwd_ok;
}

void Client::set_is_bot(bool is_bot)
{
    _is_bot = is_bot;
}

void Client::set_bot_step(int step)
{
    _bot_step = step;
}

void Client::set_registered(bool registered)
{
    _registered = registered;
}

void Client::set_hostname(std::string ip)
{
    _hostname = ip;   
}

void Client::set_msg(std::string buffer)
{
    _msg = _msg + buffer;
}

void Client::clear_msg(void)
{
    _msg.clear();
}

void Client::add_word(std::string word)
{
    _words.push_back(word);
}

void Client::clear_words(void)
{
    _words.clear();
}