/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:30:25 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/08 01:46:30 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// MODE <channel> [modestring [mode parameters...]]
// MODE #channel by any user (query of the modes of the channel)
// MODE #channel params only by operators
// IRC_ops are not channel operator by default (they must use froce to get it)
void Server::mode(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{       
    int i = tab_idx++;    
    
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "MODE " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')
        reply(COD_NOSUCHCHANNEL, tab_msg.at(i) + " " + ERR_NOSUCHCHANNEL, clt_idx);    
    else  
    {     
        std::string channel = tab_msg.at(i);        
        int chnl_idx = channel_idx(channel);        
        
        if (chnl_idx != -1)
        {
            if (i++ >= tab_msg.size()) // query by any user            
                reply(COD_CHANNELMODEIS, channel + " " + get_modes(chnl_idx, clt_idx), clt_idx); // " " + ""
            else if (_chnls.at(chnl_idx).is_operator(_clts.at(clt_idx).get_nickname())) // check it is a channel oparator
            {
                std::string minus = get_minus(tab_msg.at(i)); // ""
                std::string plus = get_plus(tab_msg.at(i)); // ""                 
                std::vector<std::string> params;                
                
                while (i++ < tab_msg.size())
                    params.push_back(tab_msg.at(i));
                if (tab_msg.at(i).at(0) == '+')                
                    (set_plus(plus, params, chnl_idx, clt_idx), set_minus(minus, params, chnl_idx, clt_idx));
                else if (tab_msg.at(i).at(0) == '-')
                    (set_minus(minus, params, chnl_idx, clt_idx), set_plus(plus, params, chnl_idx, clt_idx));
                else 
                    reply(COD_NEEDMOREPARAMS, "MODE " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
            }
            else
                reply(COD_CHANOPRIVSNEEDED, channel + " " + ERR_CHANOPRIVSNEEDED, clt_idx);            
        }
        else
            reply(COD_NOSUCHCHANNEL, channel + " " + ERR_NOSUCHCHANNEL, clt_idx);
    }
}

void Server::set_plus(std::string plus, std::vector<std::string>& params, int chnl_idx, int clt_idx)
{
    if (plus == "")
        return;
                    
    std::string allowed = "itkol";
    std::string msg_replied; // :toto!~toto@localhost MODE #test +k secret // :<nick>!<user>@<host> MODE <channel> <mode changes> <params>
    msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!~" + _clts.at(clt_idx).get_username() \
                + "@" + _clts.at(clt_idx).get_hostname() + " MODE " + _chnls.at(chnl_idx).get_name();
    
    int i = 0;    
    while (i < plus.length())
    {
        if (allowed.find(plus.at(i)) != std::string::npos)
        {
            if (plus.at(i) == 'i')
            {
                _chnls.at(chnl_idx).set_mode_i(true);
                reply_to_all(msg_replied + " +i", chnl_idx);
            }                
            if (plus.at(i) == 't')
            {
                _chnls.at(chnl_idx).set_mode_t(true);
                reply_to_all(msg_replied + " +t", chnl_idx);
            }                
            if (plus.at(i) == 'k')
            {
                if (params.size() > 0)
                {
                    if (check_key(params.at(0)) == OK)
                    {
                        _chnls.at(chnl_idx).set_key(params.at(0));
                        reply_to_all(msg_replied + " +k " + params.at(0), chnl_idx);
                    }                        
                    else
                        reply(COD_INVALIDKEY, _chnls.at(chnl_idx).get_name() + " " + ERR_INVALIDKEY, clt_idx);
                    params.erase(params.begin());
                }
                else 
                    reply(COD_NEEDMOREPARAMS, "MODE " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
            }
            if (plus.at(i) == 'o')
            {
                if (params.size() > 0)
                {
                    if (nick_in_channel(chnl_idx, params.at(0)) == OK) 
                    {
                        if (!_chnls.at(chnl_idx).is_operator(params.at(0))) // already operator --> do nothing
                        {
                            _chnls.at(chnl_idx).add_operator(params.at(0));
                            reply_to_all(msg_replied + " +o " + params.at(0), chnl_idx);                            
                        }                        
                    }                        
                    else
                        reply(COD_USERNOTINCHANNEL, params.at(0) + " " + _chnls.at(chnl_idx).get_name() + " " + ERR_USERNOTINCHANNEL, clt_idx);
                    params.erase(params.begin());
                }
                else 
                    reply(COD_NEEDMOREPARAMS, "MODE " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
            }
            if (plus.at(i) == 'l')
            {
                if (params.size() > 0)
                {
                    long lim = check_limit(params.at(0));
                    if (lim != -1)
                    {
                        _chnls.at(chnl_idx).set_limit(lim);
                        reply_to_all(msg_replied + " +l " + params.at(0), chnl_idx);
                    }                        
                    else
                        reply(COD_UNKNOWNERROR, "MODE " + params.at(0) + " " + ERR_UNKNOWNERROR, clt_idx);
                    params.erase(params.begin());
                }
                else 
                    reply(COD_NEEDMOREPARAMS, "MODE " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
            }
        }        
        else
            reply(COD_UNKNOWNMODE, plus.at(i) + " " + std::string(ERR_UNKNOWNMODE), clt_idx);
        i++;
    }        
}

void Server::set_minus(std::string minus, std::vector<std::string>& params, int chnl_idx, int clt_idx)
{
    if (minus == "")
        return;
            
    std::string allowed = "itkol";
    std::string msg_replied; // :toto!~toto@localhost MODE #test +k secret // :<nick>!<user>@<host> MODE <channel> <mode changes> <params>
    msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!~" + _clts.at(clt_idx).get_username() \
                + "@" + _clts.at(clt_idx).get_hostname() + " MODE " + _chnls.at(chnl_idx).get_name();
    
    int i = 0;    
    while (i < minus.length())
    {
        if (allowed.find(minus.at(i)) != std::string::npos)
        {
            if (minus.at(i) == 'i')
            {
                _chnls.at(chnl_idx).set_mode_i(false);
                reply_to_all(msg_replied + " -i", chnl_idx);
            }                
            if (minus.at(i) == 't')
            {
                _chnls.at(chnl_idx).set_mode_t(false);
                reply_to_all(msg_replied + " -t", chnl_idx);
            }                
            if (minus.at(i) == 'k')
            {
                _chnls.at(chnl_idx).set_key("");
                reply_to_all(msg_replied + " -k", chnl_idx);
            }                
            if (minus.at(i) == 'o')
            {
                if (params.size() > 0)
                {
                    if (nick_in_channel(chnl_idx, params.at(0)) == OK)
                    {
                        _chnls.at(chnl_idx).rem_operator(params.at(0));
                        reply_to_all(msg_replied + " -o " + params.at(0), chnl_idx);
                    }    
                    else
                        reply(COD_USERNOTINCHANNEL, params.at(0) + " " + _chnls.at(chnl_idx).get_name() + " " + ERR_USERNOTINCHANNEL, clt_idx);
                    params.erase(params.begin());
                }
                else 
                    reply(COD_NEEDMOREPARAMS, "MODE " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
            }
            if (minus.at(i) == 'l')
            {
                _chnls.at(chnl_idx).set_limit(-1);
                reply_to_all(msg_replied + " -l", chnl_idx);
            }                
        }        
        else
            reply(COD_UNKNOWNMODE, minus.at(i) + " " + std::string(ERR_UNKNOWNMODE), clt_idx);
        i++;
    }        
}

// limit affect - future - JOIN 
long Server::check_limit(std::string limit)
{
    char **endptr;
    long lim = strtol(limit.c_str(), endptr, 10);
    
    if (0 < lim && lim <= MAX_CHANNEL_LIMIT)
        return (lim);
    return (-1);
}

int Server::nick_in_channel(int chnl_idx, std::string nick)
{    
    for (int i = 0; i < _chnls.at(chnl_idx).get_chnlclts().size(); i++)
    {
        if (_chnls.at(chnl_idx).get_chnlclts().at(i).get_nickname() == nick)   
            return (OK);
    }
    return (KO);    
}

std::string Server::get_plus(std::string modestr)
{
    std::string plus;

    int i = 0;
    while (i < modestr.length() && modestr.at(i) != '+')
        i++;    
    for (int j = i + 1; j < modestr.length(); j++)
    {
        if (modestr.at(j) == '-')
            break;
        plus = plus + modestr.at(j);
    }
    return (plus);
}

std::string Server::get_minus(std::string modestr)
{
    std::string minus;

    int i = 0;
    while (i < modestr.length() && modestr.at(i) != '-')
        i++;    
    for (int j = i + 1; j < modestr.length(); j++)
    {
        if (modestr.at(j) == '+')
            break;
        minus = minus + modestr.at(j);
    }
    return (minus);
}

// operators are sent with WHO command
std::string Server::get_modes(int chnl_idx, int clt_idx)
{
    std::stringstream modes;
    std::stringstream params;
    
    modes << "+";
    if (_chnls.at(chnl_idx).get_mode_i())
        modes << "i";
    if (_chnls.at(chnl_idx).get_mode_t())
        modes << "t";
    // if (_chnls.at(chnl_idx).get_mode_k()) 
    if (!_chnls.at(chnl_idx).get_key().empty()) 
    {
        modes << "k";
        if (in_channel(chnl_idx, clt_idx) != -1) // show key if in the channel
            params << _chnls.at(chnl_idx).get_key();
    }            
    //if (_chnls.at(chnl_idx).get_mode_l())
    if (_chnls.at(chnl_idx).get_limit() != -1)
    {
        modes << "l";
        if (params != "")
            params << " " << _chnls.at(chnl_idx).get_limit();
        else     
            params << _chnls.at(chnl_idx).get_limit();
    }        
    if (modes.str() == "+")
        return ("");
    if (params != "")
        modes << " " << params;
    return (modes.str());
}

