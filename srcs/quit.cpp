/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 04:20:46 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/20 03:47:02 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// QUIT [reason] / [:reason with space]
void Server::quit(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    std::string reason;
    int pfd_idx = -1;

    if (tab_idx + 1 < tab_msg.size())
		reason = tab_msg.at(tab_idx + 1);		
	
    for (int i = 2; i < _pfds.size(); i++)
    {
        if (_pfds.at(i).fd == _clts.at(clt_idx).get_clt_skt())
        {
            pfd_idx = i;
            break;
        }            
    }    
    if (pfd_idx != -1)
    	client_disconnect(reason, pfd_idx, clt_idx);
}

// :<nickname>!<user>@<host> QUIT :[optional message]
void Server::quit_channels(std::string reason, int clt_idx)
{
	std::string msg_replied;
	
	if (!reason.empty())
		reason = " " + reason;	
		
	for (int i = 0; i < _chnls.size(); i++)	
	{		
		int idx = in_channel(i, clt_idx);
		if (idx != -1)
		{			
			_chnls.at(i).rem_operator(_clts.at(clt_idx).get_clt_skt());
			_chnls.at(i).rem_invitee(_clts.at(clt_idx).get_clt_skt());
			_chnls.at(i).rem_chnlclt(_clts.at(clt_idx).get_clt_skt());
			msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
					+ "@" + _clts.at(clt_idx).get_hostname() + " QUIT" + reason;
			reply_to_all(msg_replied, i);				
		}			
	}
	rem_empty_chnl();	
}

void Server::rem_empty_chnl(void)
{
	std::vector<Channel>::iterator it;
	
	for (it = _chnls.begin(); it != _chnls.end();)
	{
		if ((*it).get_chnlclts().size() == 0)
			it = _chnls.erase(it);  // returns next valid iterator
		else
			++it;
	}
}
