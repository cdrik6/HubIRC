/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:15:28 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/12 23:37:16 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() :
_clt_skt(-1),
_registered(false),
_pwd_ok(false),
_is_op(false),
_nickname("*"),
_username(""),
_hostname(""),
_realname(""),
_msg("")
{}

Client::~Client()
{}

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

bool Client::get_is_op(void) const
{
    return (_is_op);
}

bool Client::get_registered(void) const
{
    return (_registered);
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