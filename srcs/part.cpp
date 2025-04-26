/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:17:45 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/26 02:39:47 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// PART <channel>{,<channel>} [ :<message> ]
void Server::part(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx)
{
    size_t i = tab_idx + 1;
    std::string msg_replied;
    std::vector<std::string> channels;
    std::string reason = "";
    
    // parse channels from where to leave    
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "PART " + std::string(ERR_NEEDMOREPARAMS) , clt_idx);
    else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')        
        reply(COD_NOSUCHCHANNEL, tab_msg.at(i) + " " + ERR_NOSUCHCHANNEL, clt_idx);
    else    
        channels = split_char(tab_msg.at(i), ',');        
    
    // reason    
    i++;
    if (i < tab_msg.size())        
        reason = tab_msg.at(i);    
	// if (!reason.empty() && reason.at(0) == ':')
	// 	reason = reason.substr(1);
    if (!reason.empty())
        reason = " " + reason;
        
    // check 
    if (!channels.empty())
    {
        for (size_t j = 0; j < channels.size(); j++)        
        {
            bool is_a_chnl = false;
            for (size_t k = 0; k < _chnls.size(); k++)
            {                
                if (channels.at(j) == _chnls.at(k).get_name())
                {
                    is_a_chnl = true;
                    int idx = in_channel(k, clt_idx);
                    if (idx != -1)
                    {
                        msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
                            + "@" + _clts.at(clt_idx).get_hostname() + " PART " + channels.at(j) + reason;
                        reply_to_all(msg_replied, k); // to all including the leaving                                                
				        _chnls.at(k).rem_operator(_chnls.at(k).get_chnlclts().at(idx));
                        _chnls.at(k).rem_invitee(_chnls.at(k).get_chnlclts().at(idx));
                        _chnls.at(k).rem_chnlclt(_chnls.at(k).get_chnlclts().at(idx));
                    }
                    else
                        reply(COD_NOTONCHANNEL, channels.at(j) + " " + ERR_NOTONCHANNEL, clt_idx);
                }
            }
            if (!is_a_chnl)
                reply(COD_NOSUCHCHANNEL, channels.at(j) + " " + ERR_NOSUCHCHANNEL, clt_idx);
            rem_empty_chnl();
        }
        _replied_clts.clear();
    }        
}

void Server::reply_to_all(std::string msg_replied, int chnl_idx)
{    
    for (size_t i = 0; i < _chnls.at(chnl_idx).get_chnlclts().size(); i++)
    {
        if(_replied_clts.find(_chnls.at(chnl_idx).get_chnlclts().at(i)) == _replied_clts.end())
        {
            reply(COD_NONE, msg_replied, client_idx(_chnls.at(chnl_idx).get_chnlclts().at(i)));
            _replied_clts.insert(_chnls.at(chnl_idx).get_chnlclts().at(i));            
        }        
    }
}
