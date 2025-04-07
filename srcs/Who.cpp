/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:54:39 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/07 03:24:04 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// WHO only for #channel (only one !)
// :irc.local 352 request-toto #test listed-user1 listed-host1 irc.local listed-nick-titi H :0 Titi Realname
void Server::who(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{       
    int i = tab_idx;        
    
    i++;
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "WHO " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')
        reply(COD_NOSUCHCHANNEL, tab_msg.at(i) + " " + ERR_NOSUCHCHANNEL, clt_idx);    
    else
    {        
        std::string channel = tab_msg.at(i);
        std::string msg_replied;        
        int chnl_idx = channel_idx(channel);        
        
        if (chnl_idx != -1)        
        {
            // std::cout << _chnls.at(chnl_idx).get_chnlclts().size() << " = size\n";
            for (int j = 0; j < _chnls.at(chnl_idx).get_chnlclts().size(); j++)
            {
                int idx = client_idx(_chnls.at(chnl_idx).get_chnlclts().at(j).get_clt_skt());
                msg_replied = channel + " " + _clts.at(idx).get_username() + " " + _clts.at(idx).get_hostname();
                msg_replied = msg_replied + " ircserv " + _clts.at(idx).get_nickname();
                if (_chnls.at(chnl_idx).is_operator(_clts.at(idx).get_nickname()))
                    msg_replied = msg_replied + " H@ :0";
                else
                    msg_replied = msg_replied + " H :0";
                reply(COD_WHOREPLY, msg_replied, clt_idx);
            }        
            reply(COD_ENDOFWHO, channel + " " + RPL_ENDOFWHO, clt_idx);
        }
        else
            reply(COD_NOSUCHCHANNEL, channel + " " + ERR_NOSUCHCHANNEL, clt_idx);
    }
}
