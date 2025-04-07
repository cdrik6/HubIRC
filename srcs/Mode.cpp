/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 02:30:25 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/07 04:24:37 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// MODE <channel> [modestring [mode parameters...]]
// MODE #channel by any user (query of the modes of the channel)
// MODE #channel params only by operators
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
        std::string msg_replied;        
        int chnl_idx = channel_idx(channel);        
        
        if (chnl_idx != -1)
        {
            if (i++ >= tab_msg.size()) // query by any user            
                reply(COD_CHANNELMODEIS, channel + " " + get_modes(chnl_idx), clt_idx); // " " + ""
            else if (_chnls.at(chnl_idx).is_operator(_clts.at(clt_idx).get_nickname())) //check it is an oparator
            {
                // get_params
                // set_modes(chnl_idx);
            }
            else
                reply(COD_CHANOPRIVSNEEDED, channel + " " + ERR_CHANOPRIVSNEEDED, clt_idx);    
            
        }
        else
            reply(COD_NOSUCHCHANNEL, channel + " " + ERR_NOSUCHCHANNEL, clt_idx);    
    }
}

std::string Server::get_modes(int chnl_idx)
{
    std::stringstream modes;
    
    modes << "+";
    if (_chnls.at(chnl_idx).get_mode_i())
        modes << "i";
    if (_chnls.at(chnl_idx).get_mode_t())
        modes << "t";
    if (_chnls.at(chnl_idx).get_mode_k())
        modes << "k";
    if (_chnls.at(chnl_idx).get_mode_o())
        modes << "o";
    if (_chnls.at(chnl_idx).get_mode_l())
        modes << "l " << _chnls.at(chnl_idx).get_limit();
    if (modes.str() == "+")
        return ("");
    return (modes.str());
}

