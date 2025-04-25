/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:52:46 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/24 23:23:16 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// PRIVMSG target1,target2 :msg with spaces --> ":" mandatory
// PRIVMSG target1,target2 msgwithnospace --> ":" optionnal
void Server::privmsg(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx)
{    
    size_t i = tab_idx;
    std::vector<std::string> target;    
    std::string msg;
    std::string msg_replied;    
    
	if (i + 1 >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "PRVISMSG " + std::string(ERR_NEEDMOREPARAMS), clt_idx); // check by irssi actually, for nc
    else if (i + 2 >= tab_msg.size())
    {
        if (tab_msg.at(i + 1).at(0) == ':')
            reply(COD_NORECIPIENT, ERR_NORECIPIENT, clt_idx); // check by irssi actually, for nc
        else
            reply(COD_NOTEXTTOSEND, ERR_NOTEXTTOSEND, clt_idx); // check by irssi actually, for nc
        // Note: PRIVMSG sthgwithnospace --> 411? or 412?
        // strict RFC logic: <params> = *14( SPACE middle ) [ SPACE ":" trailing ]
        // IRC only treats something as a trailing message if it’s prefixed with ":" --> so 412
    }        
    else
    {
        msg = tab_msg.at(tab_msg.size() - 1);
        i++;
        while (i < tab_msg.size() - 1 && tab_msg.at(i).at(0) != ':') // recipients list
        {
            target = split_char(tab_msg.at(i), ','); // splitted by irssi actually, for nc
            for (size_t t = 0; t < target.size(); t++)
            {                   
                msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
                    + "@" + _clts.at(clt_idx).get_hostname() + " PRIVMSG"; // from
                msg_replied = msg_replied + " " + target.at(t) + " " + msg; // to            
                if (target.at(t).at(0) != '#' && target.at(t).at(0) != '&')
                {
                    int k = target_clt_idx(target.at(t));                    
                    if (k != -1 && _clts.at(k).get_registered())
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
                            for (size_t j = 0; j < _chnls.at(k).get_chnlclts().size(); j++)                    
                            {                                
                                int fd = _chnls.at(k).get_chnlclts().at(j);
                                if (client_idx(fd) != clt_idx) // all except itself for channel (itself ok for direct message)
                                    reply(COD_NONE, msg_replied, client_idx(fd));
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

// return where is clt_fd in _chnlclts 
int Server::in_channel(int chnl_idx, int clt_idx)
{
    for (size_t i = 0; i < _chnls.at(chnl_idx).get_chnlclts().size(); i++)
    {        
        if (_chnls.at(chnl_idx).get_chnlclts().at(i) == _clts.at(clt_idx).get_clt_skt())
            return (i);
    }
    return (-1);
}

// return clt_idx
int Server::target_clt_idx(std::string target)
{
    for (size_t i = 0; i < _clts.size(); i++)       
        if (_clts.at(i).get_nickname() == target)
            return (i);
    return (-1);
}

// return clt_fd
int Server::target_clt_fd(std::string target)
{
    for (size_t i = 0; i < _clts.size(); i++)       
        if (_clts.at(i).get_nickname() == target)
            return (_clts.at(i).get_clt_skt());
    return (-1);
}

// return index in _chnlclt not the value at _chnlclt(i)
int Server::target_chnlclt_idx(std::string target, int chnl_idx)
{
    for (size_t i = 0; i < _chnls.at(chnl_idx).get_chnlclts().size(); i++)
    {           
        if ( _clts.at(client_idx(_chnls.at(chnl_idx).get_chnlclts().at(i))).get_nickname() == target)
            return (i);            
    }        
    return (-1);
}

// return clt_fd in the channel
int Server::target_chnlclt_fd(std::string target, int chnl_idx)
{
    for (size_t i = 0; i < _chnls.at(chnl_idx).get_chnlclts().size(); i++)
    {           
        if ( _clts.at(client_idx(_chnls.at(chnl_idx).get_chnlclts().at(i))).get_nickname() == target)
            return (_chnls.at(chnl_idx).get_chnlclts().at(i));            
    }        
    return (-1);
}

int Server::channel_idx(std::string channel)
{
	for (size_t i = 0; i < _chnls.size(); i++)
	{		
		if (_chnls.at(i).get_name() == channel)			
			return (i);		
	}	
	return (-1); // disconnected in between or error ?
}
