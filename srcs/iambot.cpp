/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandm <alexandm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 00:21:15 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/18 16:16:58 by alexandm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

// IAMBOT keybot
void Server::iambot(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{       
    int i = tab_idx + 1;
    
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "IAMBOT " + std::string(ERR_NEEDMOREPARAMS), clt_idx);    
    else
    {        
        std::string key = tab_msg.at(i);       
        std::string botnick = _clts.at(clt_idx).get_nickname();
        _clts.at(clt_idx).set_is_bot(true); /*****************************to do what ? *********/
        
        if (key == BOTKEY)        
        {            
            std::string msg = ":Je suis " + botnick + " ton bot !\n Message cadavre exquis pour jouer.";
            std::string msg_bot = ":Salut le bot !";
            std::string msg_replied;            
            
            for (int i = 0; i < _clts.size(); i++) 
            {
                if (_clts.at(i).get_registered()) /***************************** correction to do in Notice and Prvmsg as well *********/
                {
                    msg_replied = ":ircserv NOTICE " + _clts.at(i).get_nickname();
                    if (i != clt_idx)                                                
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
// std::vector<std::string> bot_msg;
// bot_msg.push_back("PRIVMSG");
// bot_msg.push_back(_clts.at(i).get_nickname());
// bot_msg.push_back(":Je suis " + botnick + " ton bot ! Tape cadavre exquis pour jouer.");
// privmsg(bot_msg, i, 0); // from bot to target                 
// bot_msg.clear();