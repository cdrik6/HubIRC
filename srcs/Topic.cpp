/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:50:52 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/08 18:17:13 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


// Topic #test : --> clear the topic
void Server::topic(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    int i = tab_idx + 1;
    
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "TOPIC " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')
        reply(COD_NOSUCHCHANNEL, tab_msg.at(i) + " " + ERR_NOSUCHCHANNEL, clt_idx);    
    else
    {
        std::string channel = tab_msg.at(i);
        int chnl_idx = channel_idx(channel);
        
        if (chnl_idx != -1)
        {
            if (in_channel(chnl_idx, clt_idx) != -1)
            {
                i++;
                if (i >= tab_msg.size()) // no topic, only query by user in the channel
                {
                    if (_chnls.at(chnl_idx).get_topic() == "") 
                        reply(COD_NOTOPIC, channel + " " + RPL_NOTOPIC, clt_idx); // 331 <nickname> <channel> :No topic set
                    else
                    {
                        reply(COD_TOPIC, channel + " :" + _chnls.at(chnl_idx).get_topic(), clt_idx); // 332 <nickname> <channel> :<topic>
                        
                    } 
                        
                    // change to all
                    
                }
                    reply(COD_CHANNELMODEIS, channel + " " + get_modes(chnl_idx, clt_idx), clt_idx); // " " + ""
                
                
            }
            else
                reply(COD_NOTONCHANNEL, channel + " " + ERR_NOTONCHANNEL, clt_idx);
        }    
        else
            reply(COD_NOSUCHCHANNEL, channel + " " + ERR_NOSUCHCHANNEL, clt_idx);
    }
    
}

// topiclen = 307