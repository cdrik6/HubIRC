/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:17:45 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/07 01:18:07 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// PART <channel>{,<channel>} [ :<message> ]
void Server::part(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    int i = tab_idx;
    std::string msg_replied;
    std::vector<std::string> channels;
    std::string reason = "";    
    
    // parse channels from where to leave
    i++;
    if (i == tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "PART " + std::string(ERR_NEEDMOREPARAMS) , clt_idx);
    else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')        
        reply(COD_NOSUCHCHANNEL, tab_msg.at(i) + " " + ERR_NOSUCHCHANNEL, clt_idx);
    else    
        channels = split_char(tab_msg.at(i), ',');        
    
    // reason    
    i++;
    if (i < tab_msg.size())
        reason = " " + tab_msg.at(i);
        
    // check 
    if (!channels.empty())
    {
        for (int j = 0; j < channels.size(); j++)        
        {
            for (int k = 0; k < _chnls.size(); k++)
            {                
                if (channels.at(j) == _chnls.at(k).get_name())
                {
                    int idx = in_channel(k, clt_idx);
                    if (idx != -1)
                    {
                        msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!~" + _clts.at(clt_idx).get_username() \
                            + "@" + _clts.at(clt_idx).get_hostname() + " PART " + channels.at(j) + reason;
                        reply_to_all(msg_replied, k); // to all including the leaving                        
                        _chnls.at(k).rem_client(idx);
				        _chnls.at(k).rem_operator(_clts.at(clt_idx).get_nickname());
                    }
                    else
                        reply(COD_NOTONCHANNEL, channels.at(j) + " " + ERR_NOTONCHANNEL, clt_idx);
                }
            }
            rem_empty_chnl();
        }                
    }        
}

void Server::reply_to_all(std::string msg_replied, int chnl_idx)
{
    for (int i = 0; i < _chnls.at(chnl_idx).get_chnlclts().size(); i++)
        reply(COD_NONE, msg_replied, client_idx(_chnls.at(chnl_idx).get_chnlclts().at(i).get_clt_skt()));    
}
