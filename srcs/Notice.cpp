/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 00:24:52 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/16 03:12:22 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void notification(std::string code, std::string error);

// NOTICE target1,target2 :msg with spaces --> ":" mandatory
// NOTICE target1,target2 msgwithnospace --> ":" optionnal
void Server::notice(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{    
    int i = tab_idx;
    std::vector<std::string> target;    
    std::string msg;
    std::string msg_replied;    
    
	if (i + 1 >= tab_msg.size())
        notification(COD_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS);
    else if (i + 2 >= tab_msg.size())
    {
        if (tab_msg.at(i + 1).at(0) == ':')
            notification(COD_NORECIPIENT, ERR_NORECIPIENT);
        else
            notification(COD_NOTEXTTOSEND, ERR_NOTEXTTOSEND);
    }        
    else
    {
        msg = tab_msg.at(tab_msg.size() - 1);
        i++;
        while (i < tab_msg.size() - 1 && tab_msg.at(i).at(0) != ':') // recipients list
        {
            target = split_char(tab_msg.at(i), ','); // splitted by irssi actually, for nc
            for (int t = 0; t < target.size(); t++)
            {                   
                msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
                    + "@" + _clts.at(clt_idx).get_hostname() + " PRIVMSG"; // from
                msg_replied = msg_replied + " " + target.at(t) + " " + msg; // to            
                if (target.at(t).at(0) != '#' && target.at(t).at(0) != '&')
                {
                    int k = target_clt_idx(target.at(t));
                    if (k != -1)                    
                        reply(COD_NONE, msg_replied, k);                
                    else
                        notification(COD_NOSUCHNICK, target.at(t) + " " + ERR_NOSUCHNICK);  
                }
                else // #channel
                {                
                    int k = channel_idx(target.at(t));                
                    if (k != -1)
                    {           
                        if (in_channel(k, clt_idx) != -1)
                        {
                            for (int j = 0; j < _chnls.at(k).get_chnlclts().size(); j++)                    
                            {
                                //int idx = client_idx(_chnls.at(k).get_chnlclts().at(j)->get_clt_skt());                        
                                int idx = _chnls.at(k).get_chnlclts().at(j);
                                if (idx != clt_idx) // all except itself for channel (itself ok for direct message)
                                    reply(COD_NONE, msg_replied, idx);
                            }
                        } 
                        else
                            notification(COD_CANNOTSENDTOCHAN, target.at(t) + " " + ERR_CANNOTSENDTOCHAN);
                    }
                    else
                        notification(COD_NOSUCHCHANNEL, target.at(t) + " " + ERR_NOSUCHCHANNEL);
                }                
            }
            i++;            
        }
    }
}

void notification(std::string code, std::string error)
{
    std::cout << "NOTICE error: " << code << " " << error << std::endl;
}