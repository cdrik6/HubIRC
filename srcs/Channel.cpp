/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:50:24 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/01 23:54:19 by caguillo         ###   ########.fr       */
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