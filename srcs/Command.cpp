/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:54:37 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/03 01:14:01 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// Server --> Client reply
// RPL Format ":<server_hostname> <code> <nickname> :<message>" CRLF
void Server::reply(std::string code, std::string msg_replied, int clt_idx)
{       
    std::string rpl;    
    
    //  if (code == "000") // NICK
    //     rpl = msg_replied + _clients.at(clt_idx).get_nickname() + "\r\n";
    if (code == COD_NONE) // PRIVMSG
        rpl = msg_replied + "\r\n";
    else
        rpl = ":localhost " + code + " " + _clients.at(clt_idx).get_nickname() + " " + msg_replied + "\r\n";
    if (send(_clients.at(clt_idx).get_clt_skt(), rpl.c_str(), rpl.length(), MSG_NOSIGNAL) == - 1)
        throw (std::runtime_error("send: " + std::string(strerror(errno))));
    // ***** debug ***** //   
    std::cout << "Reply debug " << rpl << std::endl;
}

//void Server::ping(std::vector<std::string>& tab_msg, int clt_idx)
void Server::ping(int clt_idx)
{
    reply("000", "PONG localhost :", clt_idx);
    //std::string rpl = "PONG localhost :" + _clients.at(clt_idx).get_nickname() + "\r\n";
    // if (send(_clients.at(clt_idx).get_clt_skt(), rpl.c_str(), rpl.length(), MSG_NOSIGNAL) == - 1)
    //     throw (std::runtime_error("send: " + std::string(strerror(errno))));    
}

void Server::pass(std::vector<std::string>& tab_msg, int clt_idx)
{    
    int i = 0;    
    
	// verify password
    while (toUpper(tab_msg.at(i)) != "PASS")
        i++;
    i++;
	if (i == tab_msg.size())
        reply(COD_PASSWDMISMATCH, ERR_PASSWDMISSING, clt_idx);
    else if (tab_msg.at(i) == _password)    
        _clients.at(clt_idx).set_pwd_ok(true);
    else     
        reply(COD_PASSWDMISMATCH, ERR_PASSWDMISMATCH, clt_idx);	
}

// NICK command is used to give user a nickname or change the existing one
void Server::nickname(std::vector<std::string>& tab_msg, int clt_idx)
{
    int i = 0;
    std::string nick;    
    
    while (toUpper(tab_msg.at(i)) != "NICK" && i < tab_msg.size())
        i++;
    i++;
    if (i >= tab_msg.size())
        reply(COD_NONICKNAMEGIVEN, ERR_NONICKNAMEGIVEN, clt_idx);
    else
    {
        nick = tab_msg.at(i);
        if (check_nick(nick) == KO)        
            reply(COD_ERRONEUSNICKNAME, ERR_ERRONEUSNICKNAME, clt_idx);
        else if (nick_available(nick, clt_idx) == KO)
            reply(COD_NICKNAMEINUSE, ":" + nick, clt_idx);
        else
        {         
            std::string oldnick = _clients.at(clt_idx).get_nickname();            
            _clients.at(clt_idx).set_nickname(nick);            
            reply(COD_NONE, ":" + oldnick + " NICK " + nick, clt_idx);
        }
    }
}
// Note
// std::string rpl = ":" + oldnick + " NICK " + nick + "\r\n"; // simpliest
// std::string rpl = ":" + oldnick + "!~" + _clients.at(clt_idx).get_username() + "@localhost NICK " + nick + "\r\n"; --> works
// std::string rpl = "NICK " + nick + "\r\n"; --> wrong issue with oldnick not null

int Server::check_nick(std::string nick)
{
    std::string allowed = "_[]{}\\|";
    
    if (nick.length() > 30 || nick.length() == 0)
        return (KO);
    if (isdigit(nick.at(0)))
        return (KO);    
    for (int i = 0; i < nick.length(); i++)
    {           
        if(!isalnum(nick.at(i)) && allowed.find(nick.at(i)) == std::string::npos)
            return (KO);
    }        
    return (OK);
}

int Server::nick_available(std::string nick, int clt_idx)
{    
     for (int i = 0; i < _clients.size(); i++)
    {
        if (i != clt_idx)
            if (toUpper(_clients.at(i).get_nickname()) == toUpper(nick))
                return (KO);
    }
    return (OK);
}

// USER command is used at the beginning of connection to specify the username, hostname and realname of a new user
// USER <username> 0 * :<realname> // USER  username nickname hostname :realname
void Server::username(std::vector<std::string>& tab_msg, int clt_idx)
{
    std::string user;    
    int i = 0;
        
    while (toUpper(tab_msg.at(i)) != "USER")
        i++;
    i++;
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "USER " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else
    {
        user = tab_msg.at(i);        
        if (user != "")
        {
            if (_clients.at(clt_idx).get_username() != "")        
                reply(COD_ALREADYREGISTRED, ERR_ALREADYREGISTRED, clt_idx);        
            else            
                _clients.at(clt_idx).set_username(user);
        }
        else // don't happen actually 
            reply(COD_NEEDMOREPARAMS, "USER " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    }
}

// PRIVMSG <msgtarget> <text to be sent>
void Server::privmsg(std::vector<std::string>& tab_msg, int clt_idx)
{    
    int i = 0;
    std::string target;
    std::string msg;
    std::string msg_replied;
    
	while (toUpper(tab_msg.at(i)) != "PRIVMSG")
        i++;    
	if (i + 1 == tab_msg.size())
        reply(COD_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS, clt_idx); // check by irssi actually
    else if (i + 2 == tab_msg.size())
    {
        if (tab_msg.at(i + 1).at(0) == ':')
            reply(COD_NORECIPIENT, ERR_NORECIPIENT, clt_idx); // check by irssi actually
        else
            reply(COD_NOTEXTTOSEND, ERR_NOTEXTTOSEND, clt_idx); // check by irssi actually
    }        
    else
    {
        msg = tab_msg.at(tab_msg.size() - 1);
        i++;
        while (tab_msg.at(i).at(0) != ':') // recipients list
        {
            target = tab_msg.at(i);
            if (target.at(0) != '#')
            {
                int k = target_index(target);
                if (k != -1)
                {                    
                    msg_replied = ":" + _clients.at(clt_idx).get_nickname() + "!~" + _clients.at(clt_idx).get_username() + "@localhost PRIVMSG"; // from
                    msg_replied = msg_replied + " " + target + " " + msg; // to
                    reply(COD_NONE, msg_replied, k);
                }
                else
                    reply(COD_NOSUCHNICK, target + ERR_NOSUCHNICK, clt_idx);  
            }
            else
            {
                // #channel
            }            
            i++;
        }
    }
}

int Server::target_index(std::string target)
{
    for (int i = 0; i < _clients.size(); i++)       
        if (_clients.at(i).get_nickname() == target)
            return (i);
    return (-1);
}

void Server::join(std::vector<std::string>& tab_msg, int clt_idx)
{
    int i = 0;
    std::vector<std::string> channels; 
    std::vector<std::string> keys;
    
    // get channels
    while (toUpper(tab_msg.at(i)) != "JOIN")
        i++;
    i++;
    if (i == tab_msg.size())
        reply(COD_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS, clt_idx);
    else if (tab_msg.at(i) == "0")
    {
        // PART of all channels client is a member
    }
    else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')    
        reply(COD_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS, clt_idx);    
    else 
    {        
        channels = split_char(tab_msg.at(i), ',');        
        i++;        
        if (i != tab_msg.size())                    
            keys = split_char(tab_msg.at(i), ',');
        while (keys.size() < channels.size())
            keys.push_back("");
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
                    if (channels.at(j).substr(1) == _chnls.at(k).get_name())
                    {                        
                        new_chan = false;
                        if (keys.at(j) == _chnls.at(k).get_key())
                        {
                            reply_join_add(channels.at(j), k, clt_idx); // before adding it (for name list not include the new one)
                            _chnls.at(k).get_tab_clt_idx().push_back(clt_idx); // add client to channel

                            // //:<nickname>!<user>@<host> JOIN <channel>     
                            // msg_replied = ":" + _clients.at(clt_idx).get_nickname() + "!~" + _clients.at(clt_idx).get_username() + "@localhost JOIN " + channels.at(j);
					        // reply(COD_NONE, msg_replied, clt_idx);                            
                        }                            
                        else
                            reply(COD_BADCHANNELKEY, channels.at(j).substr(1) + " :wrong key to join the channel", clt_idx);
                        break;
                    }
                }
                if (new_chan == true) // create channel                
                {                    
                    reply_join_new(channels.at(j), clt_idx);
                    add_chnls(_chnls, channels.at(j).substr(1), keys.at(j), clt_idx); // create and add                    
                }    
            }
            else
                reply(COD_NOSUCHCHANNEL, channels.at(j).substr(1) + " :invalid name/key for a channel", clt_idx);
        }
    }
}

void Server::reply_join_add(std::string channel, int chnl_idx, int clt_idx)
{
    
    std::string msg_replied;
    std::string topic = _chnls.at(chnl_idx).get_topic();
    int idx;
    
    //:<nickname>!<user>@<host> JOIN <channel>     
    msg_replied = ":" + _clients.at(clt_idx).get_nickname() + "!~" + _clients.at(clt_idx).get_username() + "@localhost JOIN " + channel;    
    reply(COD_NONE, msg_replied, clt_idx);
    // All other users in #chatroom also receive this
    for (int i = 0; i < _chnls.at(chnl_idx).get_tab_clt_idx().size(); i++)    
        reply(COD_NONE, msg_replied, _chnls.at(chnl_idx).get_tab_clt_idx().at(i));
    if (topic == "") // 331 <nickname> <channel> :No topic set
        reply(COD_NOTOPIC, channel + " " + RPL_NOTOPIC, clt_idx);    
    else // 332 <nickname> <channel> :<topic>        
        reply(COD_TOPIC, channel + " :" + topic, clt_idx);   
    // 353 <nickname> = <channel> :<user1> <user2> <user3> ...
    msg_replied = "= " + channel + " :";
    for (int i = 0; i < _chnls.at(chnl_idx).get_tab_clt_idx().size(); i++)
    {
        idx = _chnls.at(chnl_idx).get_tab_clt_idx().at(i);
        if (_clients.at(idx).get_is_op())
            msg_replied = msg_replied + "@" + _clients.at(idx).get_nickname() + " ";
        else     
            msg_replied = msg_replied + _clients.at(idx).get_nickname() + " ";
    }    
    reply(COD_NAMREPLY, msg_replied, clt_idx);    
    // 366 <nickname> <channel> :End of /NAMES list    
    reply(COD_ENDOFNAMES, channel + " " + RPL_ENDOFNAMES, clt_idx);
}

void Server::reply_join_new(std::string channel, int clt_idx)
{
    std::string msg_replied;
    
    //:<nickname>!<user>@<host> JOIN <channel>     
    msg_replied = ":" + _clients.at(clt_idx).get_nickname() + "!~" + _clients.at(clt_idx).get_username() + "@localhost JOIN " + channel;
    reply(COD_NONE, msg_replied, clt_idx);
    // 331 <nickname> <channel> :No topic is set
    msg_replied = channel + " " + RPL_NOTOPIC;
    reply(COD_NOTOPIC, msg_replied, clt_idx);
    // 353 <nickname> = <channel> :<user1> <user2> <user3> ...
    msg_replied = "= " + channel + " :@" + _clients.at(clt_idx).get_nickname();
    reply(COD_NAMREPLY, msg_replied, clt_idx);
    // 366 <nickname> <channel> :End of /NAMES list    
    reply(COD_ENDOFNAMES, channel + " " + RPL_ENDOFNAMES, clt_idx);
}

void Server::add_chnls(std::vector<Channel>& chnls, std::string name, std::string key, int clt_idx)
{
    Channel new_chan;

    new_chan.set_name(name);    
    new_chan.set_key(key);
    new_chan.set_tab_clt_idx(clt_idx);
    chnls.push_back(new_chan);
}

int Server::check_channel(std::string chan_name)
{
    std::string allowed = "-_[]{}|";
    
    if (chan_name.length() > 30 || chan_name.length() == 0)
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