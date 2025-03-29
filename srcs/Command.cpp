/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:54:37 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/29 14:58:22 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// Server --> Client reply
// RPL Format ":<server_hostname> <code> <nickname> :<message>" CRLF
void Server::reply(std::string code, std::string msg_replied, int clt_idx)
{       
    std::string rpl;    
    
    // if (code == NOCODE)
    //     rpl = ":localhost " + _clients.at(client_idx).get_nickname() + ":" + msg_replied + "\r\n";
    if (code == "000") // NICK
        rpl = msg_replied + _clients.at(clt_idx).get_nickname() + "\r\n";
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
    std::string nick;
    int i = 0;    
    
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
        else if (nick_available(nick) == KO)
            reply(COD_NICKNAMEINUSE, ":" + nick, clt_idx);
        else
        {               
            // _clients.at(clt_idx).set_oldnick(_clients.at(clt_idx).get_nickname());
            std::string oldnick = _clients.at(clt_idx).get_nickname();            
            _clients.at(clt_idx).set_nickname(nick);
            reply("000", ":" + oldnick + " NICK ", clt_idx);
        }
    }
}
// Note
// std::string rpl = ":" + oldnick + " NICK " + nick + "\r\n"; // simpliest
// std::string rpl = ":" + oldnick + "!~" + _clients.at(clt_idx).get_username() + "@localhost NICK " + nick + "\r\n"; --> works
// std::string rpl = "NICK " + nick + "\r\n"; --> wrong issue with oldnick not null

int Server::check_nick(std::string nick)
{
    if (nick.length() > 20)
        return (KO);
    for (int i = 0; i < nick.length(); i++)
    {
        if(!isalnum(nick.at(i)) && nick.at(i) != '_')
            return (KO);
    }        
    return (OK);
}

int Server::nick_available(std::string nick)
{
    for (int i = 0; i < _clients.size(); i++)
    {
        if (_clients.at(i).get_nickname() == nick)
        return (KO);
    }
    return (OK);
}

// *********** rules invalid USER ????**********/////
// USER command is used at the beginning of connection to specify the username, hostname and realname of a new user
// USER <username> 0 * :<realname> // USER  username nickname localhost :realname
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
        
        if (_clients.at(clt_idx).get_username() != "")        
            reply(COD_ALREADYREGISTRED, ERR_ALREADYREGISTRED, clt_idx);        
        else            
            _clients.at(clt_idx).set_username(user);         
    }
}

// PRIVMSG <msgtarget> <text to be sent>
void Server::privmsg(std::vector<std::string>& tab_msg, int clt_idx)
{    
    int i = 0;
    std::string target;
    std::string msg;
    
	while (toUpper(tab_msg.at(i)) != "PRIVMSG")
        i++;    
	if (i + 1 == tab_msg.size() || i + 2 == tab_msg.size())
        reply(COD_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS, clt_idx);
    else
    {
        target = tab_msg.at(i + 1);
        msg = tab_msg.at(i + 2);
        if (target.at(0) != '#')
        {
          target_index(target)
          reply(COD_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS, clt_idx);  
        }    
    }
}