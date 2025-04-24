/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:13:27 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/24 15:06:03 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// :inviter INVITE <invitee> <channel> // INVITE no parameter --> not standard
void Server::invite(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    size_t i = tab_idx + 1; // invitee
    
    if (i >= tab_msg.size()) // Query invitees list, out of scope           
        reply(COD_NEEDMOREPARAMS, "INVITE " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else
    {
        std::string invitee = tab_msg.at(i);
        int inv_idx = target_clt_idx(invitee);

        if (inv_idx != -1) // invitee exists?
        {        
            i++; // channel
            if (i >= tab_msg.size())
                reply(COD_NEEDMOREPARAMS, "INVITE " + std::string(ERR_NEEDMOREPARAMS), clt_idx);        
            else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')
                reply(COD_NOSUCHCHANNEL, tab_msg.at(i) + " " + ERR_NOSUCHCHANNEL, clt_idx);    
            else
            {
                std::string channel = tab_msg.at(i);
                int chnl_idx = channel_idx(channel);
                
                if (chnl_idx != -1) // channel exits?
                {
                    if (in_channel(chnl_idx, clt_idx) != -1) // inviter in it?
                    {
                        if (in_channel(chnl_idx, inv_idx) == -1) // invitee not/already in it?
                        {                      
                            if (!_chnls.at(chnl_idx).get_mode_i()) // not restricted to operators
                                let_it_in(chnl_idx, clt_idx, inv_idx);
                            else // restricted --> irssi authorises invite for operators only so mode_i is for 
                            {                                
                                if (_chnls.at(chnl_idx).is_operator(_clts.at(clt_idx).get_clt_skt())) // only operator can set topic
                                    let_it_in(chnl_idx, clt_idx, inv_idx);
                                else
                                    reply(COD_CHANOPRIVSNEEDED, channel + " " + ERR_CHANOPRIVSNEEDED, clt_idx);                            
                            }                               
                        }
                        else
                            reply(COD_USERONCHANNEL, invitee + " " + channel + " " + ERR_USERONCHANNEL, clt_idx);    
                    }
                    else
                        reply(COD_NOTONCHANNEL, channel + " " + ERR_NOTONCHANNEL, clt_idx);
                }    
                else
                    reply(COD_NOSUCHCHANNEL, channel + " " + ERR_NOSUCHCHANNEL, clt_idx);
            }
        }
        else
            reply(COD_NOSUCHNICK, invitee + " " + ERR_NOSUCHNICK, clt_idx);
    }    
}



void Server::let_it_in(int chnl_idx, int clt_idx, int inv_idx)
{    
    // :server.name 341 <inviter> <invitee> <channel> --> to inviter
    reply(COD_INVITING, _clts.at(inv_idx).get_nickname() + " " + _chnls.at(chnl_idx).get_name(), clt_idx);

    // :inviter!user@host INVITE <invitee> <channel> --> to invitee    
    std::string msg_replied;    
    msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
                + "@" + _clts.at(clt_idx).get_hostname() + " INVITE " + _clts.at(inv_idx).get_nickname() \
                + " " + _chnls.at(chnl_idx).get_name();                
    reply(COD_NONE, msg_replied, inv_idx);   

    // update invitees list // not in the list (otherwise already in channel or delete at Part/Quit step)    
    _chnls.at(chnl_idx).add_invitee(_clts.at(inv_idx).get_clt_skt());
}
