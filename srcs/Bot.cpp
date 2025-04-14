/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 04:18:39 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/14 04:53:21 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::bot(int clt_idx)
{
    std::vector<std::string> bot_msg;
    std::stringstream ss;    
    
    bot_msg.push_back("JOIN"); // 0
    ss << "&" << time(NULL);
    bot_msg.push_back(ss.str()); // 1
    bot_msg.push_back("MODE"); // 2
    bot_msg.push_back(ss.str()); // 3
    bot_msg.push_back("+l-o 1 " + _clts.at(clt_idx).get_nickname()); // 4
    bot_msg.push_back("TOPIC"); 
    bot_msg.push_back(ss.str());
    bot_msg.push_back(":Welcome to The Segfault Exquis!"); 
    bot_msg.push_back("PRIVMSG");
    bot_msg.push_back(ss.str()); 
    bot_msg.push_back(":je te laisse commencer, donne moi un sujet");
    bot_msg.push_back("PRIVMSG");
    bot_msg.push_back(ss.str()); 
    bot_msg.push_back(":A moi, j'ai choisit un verbe");
    bot_msg.push_back("PRIVMSG");
    bot_msg.push_back(ss.str()); 
    bot_msg.push_back(":A toi, donne moi un COD ou COI");
    
    //bot_msg.push_back(":A toi, une verbe (conjugue 3e pers du singulier je commence, j'ai choisi un sujet");
    
    join(bot_msg, clt_idx, 0);
    mode(bot_msg, clt_idx, 2);
    topic(bot_msg, clt_idx, 5);
    privmsg(bot_msg, clt_idx, 8);    
    privmsg(bot_msg, clt_idx, 11);
    privmsg(bot_msg, clt_idx, 14); 
}
