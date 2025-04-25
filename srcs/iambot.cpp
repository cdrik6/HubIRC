/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iambot.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:21:15 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/24 23:22:22 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

// IAMBOT keybot
void Server::iambot(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx)
{       
    size_t i = tab_idx + 1;
    
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "IAMBOT " + std::string(ERR_NEEDMOREPARAMS), clt_idx);    
    else
    {        
        std::string key = tab_msg.at(i);       
        std::string botnick = _clts.at(clt_idx).get_nickname();
        _clts.at(clt_idx).set_is_bot(true); 
        
        if (key == BOTKEY)        
        {            
            std::string msg = ":Je suis " + botnick + " ton bot ! Invite moi et tape @" + botnick + " pour jouer.";
            std::string msg_bot = ":Salut le bot !";
            std::string msg_replied;            
            
            for (size_t i = 0; i < _clts.size(); i++) 
            {
                if (_clts.at(i).get_registered())
                {
                    msg_replied = ":ircserv NOTICE " + _clts.at(i).get_nickname();
                    if ((int)i != clt_idx)                                                
                        msg_replied = msg_replied + " " + msg;
                    else 
                        msg_replied = msg_replied + " " + msg_bot;
                    reply(COD_NONE, msg_replied, i);
                }
            }            
        }
        else
            reply(COD_PASSWDMISMATCH, key + " " + ERR_PASSWDMISMATCH, clt_idx);
    }
}
