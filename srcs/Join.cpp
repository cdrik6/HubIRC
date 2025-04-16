/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:53:44 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/16 23:03:02 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::join(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    int i = tab_idx + 1;
    std::vector<std::string> channels; 
    std::vector<std::string> keys;
    
    // get channels to join or create       
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "JOIN " + std::string(ERR_NEEDMOREPARAMS), clt_idx);    
    else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')
        reply(COD_NOSUCHCHANNEL, tab_msg.at(i) + " " + ERR_NOSUCHCHANNEL, clt_idx);
    else 
    {        
        channels = split_char(tab_msg.at(i), ',');                
        i++;        
        if (i < tab_msg.size())                    
            keys = split_char(tab_msg.at(i), ',');
        int ks = keys.size();     
        while (ks < channels.size())
        {
            keys.push_back("");
            ks = keys.size();
        }
            
    }
    
    // check existing or create channel   
    bool new_chan;    
    if (channels.size() != 0)
    {
        for (int j = 0; j < channels.size(); j++)
        {            
            if (check_channel(channels.at(j).substr(1)) == OK && check_key(keys.at(j)) == OK)
            {
                new_chan = true;
                for (int k = 0; k < _chnls.size(); k++)
                {
                    if (channels.at(j) == _chnls.at(k).get_name())
                    {                      
                        new_chan = false;
                        if (in_channel(k, clt_idx) == -1) // KO = not in// check already in the channel --> seems done by irssi, so for nc
                        { 
                            // invite, limit, key --> in that order for irssi
                            if (_chnls.at(k).get_mode_i() == false) // not invite-only
                            {                                
                                if (_chnls.at(k).get_limit() == -1 || _chnls.at(k).get_chnlclts().size() < _chnls.at(k).get_limit())
                                {                                       
                                    if (keys.at(j) == _chnls.at(k).get_key())
                                        reply_join_add(channels.at(j), k, clt_idx);
                                    else
                                        reply(COD_BADCHANNELKEY, channels.at(j) + " " + ERR_BADCHANNELKEY, clt_idx);
                                    break;
                                }
                                else if (_chnls.at(k).is_invitee(_clts.at(clt_idx).get_clt_skt()) == true)
                                    reply_join_add(channels.at(j), k, clt_idx);                                
                                else
                                    reply(COD_CHANNELISFULL, channels.at(j) + " " + ERR_CHANNELISFULL, clt_idx);
                                break;    
                            }        
                            else // invite-only bypasses limit and key if invitee // not invite-only + invitee = bypass anyway
                            {
                                // if (_chnls.at(k).is_invitee(_clts.at(clt_idx).get_nickname()) == true)                                
                                if (_chnls.at(k).is_invitee(_clts.at(clt_idx).get_clt_skt()) == true)
                                    reply_join_add(channels.at(j), k, clt_idx);                                
                                else
                                    reply(COD_INVITEONLYCHAN, channels.at(j) + " " + ERR_INVITEONLYCHAN, clt_idx);
                            }                                
                            break;                            
                        }
                        else
                            reply(COD_USERONCHANNEL, _clts.at(clt_idx).get_nickname() + " " + ERR_USERONCHANNEL, clt_idx);                            
                        break;    
                    }
                }
                if (new_chan == true) // create channel                
                {                    
                    reply_join_new(channels.at(j), clt_idx);                    
                    create_chnl(channels.at(j), keys.at(j), clt_idx); // create and add
                }    
            }
            else
                reply(COD_NOSUCHCHANNEL, channels.at(j) + " :invalid name/key for a channel", clt_idx);
        }
    }
}
// Note
// else if (tab_msg.at(i) == "0")
// PART of all channels client is a member // but impossible with irssi /JOIN 0 --> /JOIN #0    

//:<nickname>!<user>@<host> JOIN <channel>
void Server::reply_join_add(std::string channel, int chnl_idx, int clt_idx)
{
    std::string msg_replied;
    std::string topic = _chnls.at(chnl_idx).get_topic();
    int fd;    
    
    // RPL
    msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
                + "@" + _clts.at(clt_idx).get_hostname() + " JOIN " + channel;
    reply(COD_NONE, msg_replied, clt_idx);    
    reply_to_all(msg_replied, chnl_idx);
    // TOPIC
    if (topic == "") 
        reply(COD_NOTOPIC, channel + " " + RPL_NOTOPIC, clt_idx); // 331 <nickname> <channel> :No topic set
    else
    { 
        reply(COD_TOPIC, channel + " " + topic, clt_idx); // 332 <nickname> <channel> :<topic>
        std::stringstream ss;
        ss << _chnls.at(chnl_idx).get_setat();                        
        reply(COD_TOPICWHOTIME, channel + " " + _chnls.at(chnl_idx).get_setby() + " " + ss.str(), clt_idx); // 333 <channel> <nick> <setat>
    }    
    // NAMES LIST   
    msg_replied = "= " + channel + " :";    
    for (int i = 0; i < _chnls.at(chnl_idx).get_chnlclts().size(); i++)
    {        
        fd = _chnls.at(chnl_idx).get_chnlclts().at(i);        
        if (_chnls.at(chnl_idx).is_operator(fd))
            msg_replied = msg_replied + "@" + _clts.at(client_idx(fd)).get_nickname() + " ";
        else     
            msg_replied = msg_replied + _clts.at(client_idx(fd)).get_nickname() + " ";        
    }
    reply(COD_NAMREPLY, msg_replied, clt_idx); // 353 <nickname> = <channel> :<user1> <user2> <user3> ...     
    reply(COD_ENDOFNAMES, channel + " " + RPL_ENDOFNAMES, clt_idx); // 366 <nickname> <channel> :End of /NAMES list 
    // ADD
    _chnls.at(chnl_idx).set_chnlclts(_clts.at(clt_idx).get_clt_skt()); // add client to channel after (--> names list doesn't include the new one)
}
// std::cout << _chnls.at(chnl_idx).get_chnlclts().size() << " = tab clients in channel size\n";
// std::cout << idx << " = idx\n";
// std::cout << msg_replied << "\n";

//:<nickname>!<user>@<host> JOIN <channel>     
void Server::reply_join_new(std::string channel, int clt_idx)
{
    std::string msg_replied;    
    
    // RPL 
    msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
                + "@" + _clts.at(clt_idx).get_hostname() + " JOIN " + channel;
    reply(COD_NONE, msg_replied, clt_idx);    
    // TOPIC
    msg_replied = channel + " " + RPL_NOTOPIC; 
    reply(COD_NOTOPIC, msg_replied, clt_idx); // 331 <nickname> <channel> :No topic is set
    // NAMES LIST
    msg_replied = "= " + channel + " :@" + _clts.at(clt_idx).get_nickname();
    reply(COD_NAMREPLY, msg_replied, clt_idx);  // 353 <nickname> = <channel> :<user1> <user2> <user3> ...      
    reply(COD_ENDOFNAMES, channel + " " + RPL_ENDOFNAMES, clt_idx); // 366 <nickname> <channel> :End of /NAMES list    
}

void Server::create_chnl(std::string name, std::string key, int clt_idx)
{
    Channel new_chan;

    new_chan.set_name(name);    
    new_chan.set_key(key);
    // new_chan.set_chnlclts(&_clts.at(clt_idx));
    new_chan.set_chnlclts(_clts.at(clt_idx).get_clt_skt());
    // new_chan.add_operator(_clts.at(clt_idx).get_nickname()); // Add creator as operator of the channel
    new_chan.add_operator(_clts.at(clt_idx).get_clt_skt()); // Add creator as operator of the channel
    _chnls.push_back(new_chan);
}

int Server::check_channel(std::string chan_name)
{
    std::string allowed = "-_[]{}|";
    
    if (chan_name.length() > CHANLEN || chan_name.length() == 0)
        return (KO);    
    for (int i = 0; i < chan_name.length(); i++)
    {           
        if(!isalnum(chan_name.at(i)) && allowed.find(chan_name.at(i)) == std::string::npos)
            return (KO);
    }        
    if (chan_name == "opers" || chan_name == "services")
        return (KO);    
    return (OK);
}

int Server::check_key(std::string key_name)
{
    std::string allowed = "-_[]{}|+*%$()=";
    
    if (key_name == "")
        return (OK);
    if (key_name.length() > 32)
        return (KO);    
    for (int i = 0; i < key_name.length(); i++)
    {           
        if(!isalnum(key_name.at(i)) && allowed.find(key_name.at(i)) == std::string::npos)
            return (KO);
    }
    return (OK);
}
