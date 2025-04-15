/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 04:18:39 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/15 03:36:40 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::bot(int clt_idx)
{
    Client bot;
    int bot_idx;
    // std::vector<std::string> bot_msg;
    
    bot.set_is_bot(true);
    bot.set_pwd_ok(true);
    bot.set_registered(true);    
    bot.set_nickname(create_botnick());
    bot.set_username("ibot"),
    bot.set_hostname("hostbot");
    bot.set_realname("ro bot"),
    _clts.push_back(bot);
    //    
    bot_idx = _clts.size() - 1;    
    msg_from_bot(":Bienvenue dans le segfault boit le clavier comme une girafe", bot_idx, clt_idx);
    msg_from_bot(":donnez-moi un verbe (3e p.s.)", bot_idx, clt_idx);
    
    // step 0 - welcome
    // bot_msg.push_back("PRIVMSG");
    // bot_msg.push_back(_clts.at(clt_idx).get_nickname()); // target
    // bot_msg.push_back(":Bienvenue dans le segfault boit le clavier comme une girafe");
    // privmsg(bot_msg, bot_idx, 0); // from bot to target
    // bot_msg.clear();    
    // step 0 - first word
    // _clts.at(bot_idx).add_word("le sujet");
    // to step 1
    // bot_msg.push_back("PRIVMSG");
    // bot_msg.push_back(_clts.at(clt_idx).get_nickname()); // target
    // bot_msg.push_back(":donnez-moi un verbe (3e p.s.)");
    // privmsg(bot_msg, bot_idx, 0); // from bot to target
    // bot_msg.clear();   
    
    
    
    
    // std::stringstream ss;
    // bot_msg.push_back("JOIN"); // 0
    // ss << "&" << time(NULL);
    // bot_msg.push_back(ss.str()); // 1
    // bot_msg.push_back("MODE"); // 2
    // bot_msg.push_back(ss.str()); // 3
    // bot_msg.push_back("+l-o 1 " + _clts.at(clt_idx).get_nickname()); // 4
    // bot_msg.push_back("TOPIC"); 
    // bot_msg.push_back(ss.str());
    // bot_msg.push_back(":Welcome to The Segfault Exquis!"); 
    // bot_msg.push_back("PRIVMSG");
    // bot_msg.push_back(ss.str()); 
    // bot_msg.push_back(":je te laisse commencer, donne moi un sujet");
    // bot_msg.push_back("PRIVMSG");
    // bot_msg.push_back(ss.str()); 
    // bot_msg.push_back(":A moi, j'ai choisit un verbe");
    // bot_msg.push_back("PRIVMSG");
    // bot_msg.push_back(ss.str()); 
    // bot_msg.push_back(":A toi, donne moi un COD ou COI");
    
    //bot_msg.push_back(":A toi, une verbe (conjugue 3e pers du singulier je commence, j'ai choisi un sujet");
    
    // join(bot_msg, clt_idx, 0);
    // mode(bot_msg, clt_idx, 2);
    // topic(bot_msg, clt_idx, 5);
    // privmsg(bot_msg, clt_idx, 8);    
    // privmsg(bot_msg, clt_idx, 11);
    // privmsg(bot_msg, clt_idx, 14); 
}

std::string Server::create_botnick(void)
{
    int k = 0;
    
    for (int i = 0; i < _clts.size(); i++)
    {
        if (_clts.at(i).get_nickname().substr(0,3) == "bot")
            k++;
    }
    // if (k != 0)
    // {
        std::stringstream ss;
        ss << "bot" << k;
        return (ss.str());    
    // }        
    // return ("bot"); // Do not return bot as it is the name command
}

std::string Server::build_sentence(int bot_idx, std::string word)
{    
    if (_clts.at(bot_idx).get_bot_step() == 0)
    {   
        _clts.at(bot_idx).add_word("le sujet");
        _clts.at(bot_idx).add_word(word); // verbe
        _clts.at(bot_idx).add_word("le COD/COI"); // COD/COI
        _clts.at(bot_idx).set_bot_step(1);
        return (":donne moi un CC maintenant");
    }        
    if (_clts.at(bot_idx).get_bot_step() == 1)
    {
        std::string sentence;
        _clts.at(bot_idx).add_word(word); // CC 
        std::cout << "je suis word " << word << std::endl;
        for (int i = 0; i < _clts.at(bot_idx).get_words().size(); i++)       
        {
            std::cout << _clts.at(bot_idx).get_words().at(i) << std::endl;
        }    
        sentence = ":" + _clts.at(bot_idx).get_words().at(0) + \
                   " " + _clts.at(bot_idx).get_words().at(1) + \
                   " " + _clts.at(bot_idx).get_words().at(2) + \
                   " " + _clts.at(bot_idx).get_words().at(3);
        _clts.at(bot_idx).set_bot_step(0);
        _clts.at(bot_idx).clear_words();        
        return (sentence);
    }
    return ("");
}

void Server::msg_from_bot(std::string msg, int bot_idx, int clt_idx)
{
    std::vector<std::string> bot_msg;
    
    bot_msg.push_back("PRIVMSG");
    bot_msg.push_back(_clts.at(clt_idx).get_nickname()); // target
    bot_msg.push_back(msg);
    privmsg(bot_msg, bot_idx, 0); // from bot to target
    // bot_msg.clear();   
}

// std::string Server::step1(Client bot, int clt_idx)
// {
//     std::vector<std::string> bot_msg;
    
//     bot_msg.push_back("PRIVMSG");
//     bot_msg.push_back(_clts.at(clt_idx).get_nickname());
//     bot_msg.push_back(":choisi le sujet de la phrase, a toi le verbe :");
//     privmsg(bot_msg, clt_idx, 0);
//     bot_msg.clear();    
// }

// std::string Server::step2(Client bot, int clt_idx)
// {
//     std::vector<std::string> bot_msg;
    
//     bot_msg.push_back("PRIVMSG");
//     bot_msg.push_back(_clts.at(clt_idx).get_nickname());
//     bot_msg.push_back(":choisi le sujet de la phrase, a toi le verbe :");
//     privmsg(bot_msg, clt_idx, 0);
//     bot_msg.clear();    
// }