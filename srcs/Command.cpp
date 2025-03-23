/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:54:37 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/23 01:30:37 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

// NICK command is used to give user a nickname or change the existing one
void Server::nickname(int client_idx)
{
    std::vector<std::string> tab_msg;
    std::string nick;
    int i = 0;
    
    tab_msg = split(_clients.at(client_idx).get_msg());    
    while (tab_msg.at(i) != "NICK" && tab_msg.at(i) != "nick")
        i++;
    i++;
    if (i == tab_msg.size())
        // reply(ERR_NONICKNAMEGIVEN());
        reply(":localhost 431 :No nickname given\r\n", client_idx);
    else
    {
        nick = tab_msg.at(i);
        reply(":mangue!~mangue@127.0.0.1 NICK :mangue\r\n", client_idx);
        
        reply(":localhost 001 mangue :Welcome to the Internet Relay Network mangue!~mangue@127.0.0.1\r\n", client_idx);
        //reply(":localhost 002 :Your host is toto, running version 0", client_idx);
        // reply(":localhost 003 :This server was created 01/01/2025", client_idx);
        // reply(":localhost 004 :toto 0 0 0", client_idx);
        //reply(":localhost 001 " + nick + ":Welcome to the Internet Relay Network", client_idx);
        // :punch.wa.us.dal.net 001 mangue :Welcome to the DALnet IRC Network mangue!~mangue@37.169.60.35

        reply(":localhost 002 mangue :Your host is localhost, running version bahamut-2.2.3\r\n", client_idx);
        reply(":localhost 003 mangue :This server was created Mon Dec 11 2023 at 20:33:25 UTC\r\n", client_idx);
        reply(":localhost 004 mangue localhost bahamut-2.2.3 aAbcCdefFghHiIjkKmnoOPrRsSwxXy AbceiIjklLmMnoOpPrRsStv beIjklov\r\n", client_idx);
        reply(":localhost 302 :nothing\r\n", client_idx);
        reply(":localhost 375 mangue :- localhost Message of the Day -\r\n", client_idx);
        reply(":localhost 372 mangue :- Welcome to my IRC server!\r\n", client_idx);
        reply(":localhost 376 mangue :End of /MOTD command.\r\n", client_idx);

    }
        
    // 001    RPL_WELCOME
    // "Welcome to the Internet Relay Network
    //  <nick>!<user>@<host>"
    
    
}

/********** try catch  to do ********/
// Server --> Client reply
void Server::reply(std::string rpl_err, int client_idx)
{    
    if (send(_clients.at(client_idx).get_clt_skt(), rpl_err.c_str(), rpl_err.length(), MSG_NOSIGNAL) == - 1)
        throw (std::runtime_error("send: " + std::string(strerror(errno))));
        
    std::cout << "Reply " << rpl_err << std::endl;
    	
}