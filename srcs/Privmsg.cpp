/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:52:46 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/07 03:07:59 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// PRIVMSG <msgtarget> <text to be sent>
void Server::privmsg(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{    
    int i = tab_idx;
    std::vector<std::string> target;    
    std::string msg;
    std::string msg_replied;
    
	// while (toUpper(tab_msg.at(i)) != "PRIVMSG")
    //     i++;    
	if (i + 1 == tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "PRVISMSG " + std::string(ERR_NEEDMOREPARAMS), clt_idx); // check by irssi actually, for nc
    else if (i + 2 == tab_msg.size())
    {
        if (tab_msg.at(i + 1).at(0) == ':')
            reply(COD_NORECIPIENT, ERR_NORECIPIENT, clt_idx); // check by irssi actually, for nc
        else
            reply(COD_NOTEXTTOSEND, ERR_NOTEXTTOSEND, clt_idx); // check by irssi actually, for nc
    }        
    else
    {
        msg = tab_msg.at(tab_msg.size() - 1);
        i++;
        while (tab_msg.at(i).at(0) != ':') // recipients list
        {
            target = split_char(tab_msg.at(i), ','); // splitted by irssi actually, for nc
            for (int t = 0; t < target.size(); t++)
            {                   
                msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!~" + _clts.at(clt_idx).get_username() \
                    + "@" + _clts.at(clt_idx).get_hostname() + " PRIVMSG"; // from
                msg_replied = msg_replied + " " + target.at(t) + " " + msg; // to            
                if (target.at(t).at(0) != '#' && target.at(t).at(0) != '&')
                {
                    int k = target_index(target.at(t));
                    if (k != -1)                    
                        reply(COD_NONE, msg_replied, k);                
                    else
                        reply(COD_NOSUCHNICK, target.at(t) + " " + ERR_NOSUCHNICK, clt_idx);  
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
                                int idx = client_idx(_chnls.at(k).get_chnlclts().at(j).get_clt_skt());                        
                                if (idx != clt_idx)
                                    reply(COD_NONE, msg_replied, idx);
                            }
                        } 
                        else
                            reply(COD_CANNOTSENDTOCHAN, target.at(t) + " " + ERR_CANNOTSENDTOCHAN, clt_idx);
                    }
                    else
                        reply(COD_NOSUCHCHANNEL, target.at(t) + " " + ERR_NOSUCHCHANNEL, clt_idx);
                }                
            }
            i++;            
        }
    }
}

int Server::in_channel(int chnl_idx, int clt_idx)
{
    for (int i = 0; i < _chnls.at(chnl_idx).get_chnlclts().size(); i++)
    {
        if (_chnls.at(chnl_idx).get_chnlclts().at(i).get_nickname() == _clts.at(clt_idx).get_nickname())   
            return (i);
    }
    return (-1);
}

int Server::target_index(std::string target)
{
    for (int i = 0; i < _clts.size(); i++)       
        if (_clts.at(i).get_nickname() == target)
            return (i);
    return (-1);
}

int Server::channel_idx(std::string channel)
{
	for (int i = 0; i < _chnls.size(); i++)
	{		
		if (_chnls.at(i).get_name() == channel)			
			return (i);		
	}	
	return (-1); // disconnected in between or error ?
}

/**** draft */
// // PRIVMSG <msgtarget> <text to be sent>
// void Server::privmsg(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
// {    
//     int i = tab_idx;    
//     std::string target;
//     std::string msg;
//     std::string msg_replied;
    
// 	// while (toUpper(tab_msg.at(i)) != "PRIVMSG")
//     //     i++;    
// 	if (i + 1 == tab_msg.size())
//         reply(COD_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS, clt_idx); // check by irssi actually, for nc
//     else if (i + 2 == tab_msg.size())
//     {
//         if (tab_msg.at(i + 1).at(0) == ':')
//             reply(COD_NORECIPIENT, ERR_NORECIPIENT, clt_idx); // check by irssi actually, for nc
//         else
//             reply(COD_NOTEXTTOSEND, ERR_NOTEXTTOSEND, clt_idx); // check by irssi actually, for nc
//     }        
//     else
//     {
//         msg = tab_msg.at(tab_msg.size() - 1);
//         i++;
//         while (tab_msg.at(i).at(0) != ':') // recipients list
//         {
//             target = tab_msg.at(i); //split_char(tab_msg.at(i), ','); // splitted by irssi actually for nc
//             std::cout << "target = " << target << " i = " << i << std::endl;
            
//             msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!~" + _clts.at(clt_idx).get_username() + "@localhost PRIVMSG"; // from
//             msg_replied = msg_replied + " " + target + " " + msg; // to            
//             if (target.at(0) != '#' && target.at(0) != '&')
//             {
//                 int k = target_index(target);
//                 if (k != -1)                    
//                     reply(COD_NONE, msg_replied, k);                
//                 else
//                     reply(COD_NOSUCHNICK, target + " " + ERR_NOSUCHNICK, clt_idx);  
//             }
//             else // #channel
//             {                
//                 int k = channel_idx(target);                
//                 if (k != -1)
//                 {           
//                     if (in_channel(k, clt_idx) == OK)
//                     {
//                         for (int j = 0; j < _chnls.at(k).get_chnlclts().size(); j++)                    
//                         {
//                             int idx = client_idx(_chnls.at(k).get_chnlclts().at(j).get_clt_skt());                        
//                             if (idx != clt_idx)
//                                 reply(COD_NONE, msg_replied, idx);
//                         }
//                     } 
//                     else
//                         reply(COD_CANNOTSENDTOCHAN, target + " " + ERR_CANNOTSENDTOCHAN, clt_idx);
//                 }
//                 else
//                     reply(COD_NOSUCHCHANNEL, target + " " + ERR_NOSUCHCHANNEL, clt_idx);
//             }            
//             i++;
//             std::cout << "je suis la ++ " << target << " i = " << i << std::endl;
//         }
//     }
// }