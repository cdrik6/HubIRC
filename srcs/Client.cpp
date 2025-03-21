/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 23:15:28 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/21 02:39:26 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

int Client::get_clt_skt(void) const
{
    return(_clt_skt);
}

std::string Client::get_msg(void) const
{
    return(_msg);
}

void Client::set_clt_skt(int clt_skt)
{
    _clt_skt = clt_skt;
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