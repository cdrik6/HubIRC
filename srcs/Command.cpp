/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:54:37 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/24 03:37:37 by caguillo         ###   ########.fr       */
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
        rpl = ":localhost " + code + " " + _clients.at(client_idx).get_nickname() + " :" + msg_replied + "\r\n";    
    if (send(_clients.at(client_idx).get_clt_skt(), rpl.c_str(), rpl.length(), MSG_NOSIGNAL) == - 1)
        throw (std::runtime_error("send: " + std::string(strerror(errno))));
    //    
    std::cout << "Reply " << rpl << std::endl;    	
}

// NICK command is used to give user a nickname or change the existing one
void Server::nickname(std::vector<std::string>& tab_msg, int client_idx)
{
    std::string nick;
    int i = 0;
    
    // tab_msg = split(_clients.at(client_idx).get_msg());    
    while (toUpper(tab_msg.at(i)) != "NICK")
        i++;
    i++;
    if (i == tab_msg.size())
        reply(ERR_NONICKNAMEGIVEN, RPL_NONICKNAMEGIVEN, client_idx);
    else
    {
        nick = tab_msg.at(i);
        if (check_nick(nick) == KO)        
            reply(ERR_ERRONEUSNICKNAME, RPL_ERRONEUSNICKNAME, client_idx);
        else if (nick_available(nick) == KO)
            reply(ERR_NICKNAMEINUSE, RPL_NICKNAMEINUSE, client_idx);
        else
        {
            //_clients.at(client_idx).get_nickname()
            _clients.at(client_idx).set_nickname(nick);               
            // reply something ???? reply(NOCODE, RPL_NICK, client_idx);
            // if ()
            
        }
            
            
        
        
        // reply(":localhost 001 mangue :Welcome to the Internet Relay Network mangue!~mangue@127.0.0.1\r\n", client_idx);
        // //reply(":localhost 002 :Your host is toto, running version 0", client_idx);
        // // reply(":localhost 003 :This server was created 01/01/2025", client_idx);
        // // reply(":localhost 004 :toto 0 0 0", client_idx);
        // //reply(":localhost 001 " + nick + ":Welcome to the Internet Relay Network", client_idx);
        // // :punch.wa.us.dal.net 001 mangue :Welcome to the DALnet IRC Network mangue!~mangue@37.169.60.35

        // reply(":localhost 002 mangue :Your host is localhost, running version bahamut-2.2.3\r\n", client_idx);
        // reply(":localhost 003 mangue :This server was created Mon Dec 11 2023 at 20:33:25 UTC\r\n", client_idx);
        // reply(":localhost 004 mangue localhost bahamut-2.2.3 aAbcCdefFghHiIjkKmnoOPrRsSwxXy AbceiIjklLmMnoOpPrRsStv beIjklov\r\n", client_idx);
        
        // reply(":localhost 375 mangue :- localhost Message of the Day -\r\n", client_idx);
        // reply(":localhost 372 mangue :- Welcome to my IRC server!\r\n", client_idx);
        // reply(":localhost 376 mangue :End of /MOTD command.\r\n", client_idx);

    }
        
    // 001    RPL_WELCOME
    // "Welcome to the Internet Relay Network
    //  <nick>!<user>@<host>"    
}

int Server::check_nick(std::string nick)
{

    return(OK);
}

int Server::nick_available(std::string nick)
{

    return(OK);
}