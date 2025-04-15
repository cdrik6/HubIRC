/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Register.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:54:37 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/15 20:45:35 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Register.hpp"

// Server --> Client reply
// RPL Format ":<server_hostname> <code> <nickname> :<message>" CRLF
// Reply TO clt_idx
void Server::reply(std::string code, std::string msg_replied, int clt_idx)
{       
    std::string rpl;
    
    if (code == COD_NONE)
        rpl = msg_replied + "\r\n";
    else
        rpl = ":ircserv " + code + " " + _clts.at(clt_idx).get_nickname() + " " + msg_replied + "\r\n";
    if (send(_clts.at(clt_idx).get_clt_skt(), rpl.c_str(), rpl.length(), MSG_NOSIGNAL) == - 1)
        throw (std::runtime_error("send: " + std::string(strerror(errno))));
    // Server output
    std::cout << "Replied("<< _clts.at(clt_idx).get_clt_skt() << ") " << rpl << std::endl;    
}

// PONG: [<server>] <token>
void Server::ping(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    int i = tab_idx + 1;
        
	if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "PING " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else        
        reply(COD_NONE, "PONG " + tab_msg.at(i), clt_idx);            
}

void Server::pass(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{    
    int i = tab_idx + 1;
    
	// verify password    
	if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "PASS " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else if (tab_msg.at(i) == _password)    
        _clts.at(clt_idx).set_pwd_ok(true);
    else     
        reply(COD_PASSWDMISMATCH, ERR_PASSWDMISMATCH, clt_idx);	
}

// NICK command is used to give user a nickname or change the existing one
void Server::nickname(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    int i = tab_idx + 1;
    std::string nick;    
        
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
            std::string oldnick = _clts.at(clt_idx).get_nickname();
            // // change nick in all channels
            // for (int k = 0; k < _chnls.size(); k++)
            // {
            //     for (int j = 0; j < _chnls.at(k).get_chnlclts().size(); j++)
            //     {
            //         if (_chnls.at(k).get_chnlclts().at(j).get_nickname() == oldnick)
            //         {
            //             std::cout << " je suis la  old = " << oldnick << std::endl;
            //             _chnls.at(k).getref_chnlclts().at(j).set_nickname(nick);
            //             break;
            //         }                        
            //     }
            // }
            for (int k = 0; k < _chnls.size(); k++)
            {
                for (int j = 0; j < _chnls.at(k).get_chnlclts().size(); j++)
                {
                    // if (_chnls.at(k).get_chnlclts().at(j).get_nickname() == oldnick)
                    // {
                        
                        std::cout << "nick registered = " << _chnls.at(k).get_chnlclts().at(j)->get_nickname() << std::endl; 
                        
                    // }                        
                }
            }            
            _clts.at(clt_idx).set_nickname(nick);            
            reply(COD_NONE, ":" + oldnick + " NICK " + nick, clt_idx);
        }
    }
}
// Note
// std::string rpl = ":" + oldnick + " NICK " + nick + "\r\n"; // simpliest
// std::string rpl = ":" + oldnick + "!" + _clients.at(clt_idx).get_username() + "@localhost NICK " + nick + "\r\n"; --> works
// std::string rpl = "NICK " + nick + "\r\n"; --> wrong issue with oldnick not null

int Server::check_nick(std::string nick)
{
    std::string allowed = "_[]{}\\|";
    
    if (nick.length() > NICKLEN || nick.length() == 0)
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
    for (int i = 0; i < _clts.size(); i++)
    {
        if (i != clt_idx)
            if (toUpper(_clts.at(i).get_nickname()) == toUpper(nick))
                return (KO);
    }
    return (OK);
}

// USER command is used at the beginning of connection to specify the username, hostname and realname of a new user
// USER <username> 0 * :<realname> // USER  username nickname hostname :realname
void Server::username(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    int i = tab_idx;    
    std::string user;
    std::string real;    
        
    if (i + 4 >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "USER " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else
    {
        user = tab_msg.at(i + 1);
        real = tab_msg.at(i + 4);  
        if (!real.empty() && real.at(0) == ':')
			real = real.substr(1);        
        if (check_user(user) == OK)
        {        
            if (_clts.at(clt_idx).get_username() != "")        
                reply(COD_ALREADYREGISTRED, ERR_ALREADYREGISTRED, clt_idx);        
            else
            {
                _clts.at(clt_idx).set_username("~" + user); // "~" should be before username as no Ident Protocol is in place.
                _clts.at(clt_idx).set_realname(real);
            }
        }
        else
            reply(COD_INVALIDUSERNAME, ERR_INVALIDUSERNAME, clt_idx);
    }        
}

int Server::check_user(std::string user)
{
    std::string allowed = "_[]{}\\|";
    
    if (user.length() > USERLEN || user.length() == 0)
        return (KO);
    if (isdigit(user.at(0)))
        return (KO);    
    for (int i = 0; i < user.length(); i++)
    {           
        if(!isalnum(user.at(i)) && allowed.find(user.at(i)) == std::string::npos)
            return (KO);
    }        
    return (OK);
}

void Server::welcome(int clt_idx)
{
	std::string msg_replied;
	
	_clts.at(clt_idx).set_registered(true);	
	msg_replied = std::string(RPL_WELCOME) + " " + _clts.at(clt_idx).get_nickname() \
				+ "!" + _clts.at(clt_idx).get_username() + "@" + _clts.at(clt_idx).get_hostname(); //  <nick>!<user>@<host>"    
	reply(COD_WELCOME, msg_replied, clt_idx);
	reply(COD_YOURHOST, RPL_YOURHOST, clt_idx);
	reply(COD_CREATED, RPL_CREATED, clt_idx);
	reply(COD_MYINFO, RPL_MYINFO, clt_idx);	// <available user modes> <available channel modes>
	reply(COD_MOTDSTART, RPL_MOTDSTART, clt_idx);
	reply(COD_MOTD, RPL_MOTD, clt_idx);
	reply(COD_ENDOFMOTD, RPL_ENDOFMOTD, clt_idx);
}
