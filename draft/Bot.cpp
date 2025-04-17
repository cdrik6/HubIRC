/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 04:18:39 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/17 03:38:20 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::bot(int clt_idx)
{
    Client bot;
    int bot_idx;    
        
    bot.set_is_bot(true);
    bot.set_pwd_ok(true);
    bot.set_registered(true);    
    bot.set_nickname(create_botnick());
    bot.set_username("ibot"),
    bot.set_hostname("hostbot");
    bot.set_realname("ro bot"),
    bot.set_clt_skt(create_bot_skt());
    _clts.push_back(bot);        
    bot_idx =  target_clt_idx(bot.get_nickname()); // _clts.size() - 1;         
    //
    std::string botchan = create_botchan_name();
    bot_join(botchan, clt_idx);
    // std::string topic = ":The Segfault Exquis!";
    // bot_topic(topic, botchan, clt_idx);    
    // msg_from_bot(":donnez-moi un verbe (3e p.s.)", botchan, bot_idx);    
}

std::string Server::create_botnick(void)
{
    int k = 0;
    
    for (int i = 0; i < _clts.size(); i++)
    {
        if (_clts.at(i).get_nickname().substr(0,3) == "bot")
            k++;
    }    
    std::stringstream ss;
    ss << "bot" << k;
    return (ss.str());
}

std::string Server::create_botchan_name(void)
{
    int k = 0;
    
    for (int i = 0; i < _chnls.size(); i++)
    {
        if (_chnls.at(i).get_name().substr(0,3) == "bot")
            k++;
    }    
    std::stringstream ss;
    ss << "#botchan" << k;
    return (ss.str());
}

int Server::create_bot_skt(void)
{
    int k = -2;
    
    for (int i = 0; i < _clts.size(); i++)
    {
        if (_clts.at(i).get_clt_skt() < 0)
            k--;
    }
    return (k);
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

void Server::msg_from_bot(std::string msg, std::string botchan, int bot_idx)
{
    std::vector<std::string> bot_msg;
    
    bot_msg.push_back("PRIVMSG");
    // bot_msg.push_back(_clts.at(clt_idx).get_nickname()); // target
    bot_msg.push_back(botchan); // target
    bot_msg.push_back(msg);
    privmsg(bot_msg, bot_idx, 0); // from bot to target 
}

void Server::bot_topic(std::string msg, std::string botchan, int clt_idx)
{
    std::vector<std::string> bot_msg;
    
    bot_msg.push_back("TOPIC");
    bot_msg.push_back(botchan);
    bot_msg.push_back(msg);
    topic(bot_msg, clt_idx, 0);
}

void Server::bot_join(std::string botchan, int clt_idx)
{
    std::vector<std::string> bot_msg;
    
    bot_msg.push_back("JOIN");
    bot_msg.push_back(botchan);
    join(bot_msg, clt_idx, 0);
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
