/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:54:37 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/25 02:10:31 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/********** try catch  to do **********************************/
// Server --> Client reply
// RPL Format ":<server_hostname> <code> <nickname> :<message>" CRLF
void Server::reply(std::string code, std::string msg_replied, int client_idx)
{       
    std::string rpl;

    // if (code == NOCODE)
    //     rpl = ":localhost " + _clients.at(client_idx).get_nickname() + ":" + msg_replied + "\r\n";    
    // else
        rpl = ":localhost " + code + " " + _clients.at(client_idx).get_nickname() + " " + msg_replied + "\r\n";    
    if (send(_clients.at(client_idx).get_clt_skt(), rpl.c_str(), rpl.length(), MSG_NOSIGNAL) == - 1)
        throw (std::runtime_error("send: " + std::string(strerror(errno))));
    // ***** debug ***** //   
    std::cout << "Reply debug " << rpl << std::endl;    	
}

// void Server::cap(std::vector<std::string>& tab_msg, int client_idx)
// {    
//     if (tab_msg.size() > 2)
//         _clients.at(client_idx).set_cap_alone(false);
// }

void Server::ping(std::vector<std::string>& tab_msg, int clt_idx)
{    
    std::string rpl = "PONG localhost";
    	
    if (send(_clients.at(clt_idx).get_clt_skt(), rpl.c_str(), rpl.length(), MSG_NOSIGNAL) == - 1)
        throw (std::runtime_error("send: " + std::string(strerror(errno))));
    // ***** debug ***** //   
    std::cout << "Reply debug " << rpl << std::endl;    	
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
    
    while (toUpper(tab_msg.at(i)) != "NICK")
        i++;
    i++;
    if (i == tab_msg.size())
        reply(COD_NONICKNAMEGIVEN, ERR_NONICKNAMEGIVEN, clt_idx);
    else
    {
        nick = tab_msg.at(i);
        if (check_nick(nick) == KO)        
            reply(COD_ERRONEUSNICKNAME, ERR_ERRONEUSNICKNAME, clt_idx);
        else if (nick_available(nick) == KO)
            reply(COD_NICKNAMEINUSE, ERR_NICKNAMEINUSE, clt_idx);
        else
        {          
            if (_clients.at(clt_idx).get_nickname() == "*") // Set New Nick            
                _clients.at(clt_idx).set_nickname(nick); //reply(NOCODE, RPL_NICK, client_idx); ???                
            else //change Nick
            {
                std::string oldnick = _clients.at(clt_idx).get_nickname();
                _clients.at(clt_idx).set_nickname(nick);
                std::string rpl = ":" + oldnick + "!" + _clients.at(clt_idx).get_username() + "@localhost NICK " + nick + "\r\n";
                if (send(_clients.at(clt_idx).get_clt_skt(), rpl.c_str(), rpl.length(), MSG_NOSIGNAL) == - 1)
                    throw (std::runtime_error("send: " + std::string(strerror(errno))));
                // ***** debug ***** //
                std::cout << "Reply debug " << rpl << std::endl;
            }
        }         
    }
}

int Server::check_nick(std::string nick)
{

    return(OK);
}

int Server::nick_available(std::string nick)
{

    return(OK);
}

// *********** rules invalid USER ????**********/////
// USER command is used at the beginning of connection to specify the username, hostname and realname of a new user
void Server::username(std::vector<std::string>& tab_msg, int clt_idx)
{
    std::string user;    
    int i = 0;
    
    // tab_msg = split(_clients.at(client_idx).get_msg());    
    while (toUpper(tab_msg.at(i)) != "USER")
        i++;
    i++;
    if (i == tab_msg.size())
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