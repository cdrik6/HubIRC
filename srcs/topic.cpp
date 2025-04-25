/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:50:52 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/24 23:24:03 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Topic #test : --> clear the topic
void Server::topic(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx)
{
    size_t i = tab_idx + 1;
    
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
                if (i >= tab_msg.size()) // only query by user in the channel
                {
                    if (_chnls.at(chnl_idx).get_topic() == "") 
                        reply(COD_NOTOPIC, channel + " " + RPL_NOTOPIC, clt_idx);
                    else
                    {
                        reply(COD_TOPIC, channel + _chnls.at(chnl_idx).get_topic(), clt_idx);
                        std::stringstream ss;
                        ss << _chnls.at(chnl_idx).get_setat();
                        reply(COD_TOPICWHOTIME, channel + " " + _chnls.at(chnl_idx).get_setby() + " " + ss.str(), clt_idx); // 333 <channel> <nick> <setat>
                    }
                }
                else // set or clear topic
                {
                    if (tab_msg.at(i).length() <= TOPICLEN)
                    {                             
                        if (!_chnls.at(chnl_idx).get_mode_t()) // not restricted to operators                        
                            update_topic(tab_msg.at(i), chnl_idx, clt_idx);                        
                        else // restricted
                        {
                            if (_chnls.at(chnl_idx).is_operator(_clts.at(clt_idx).get_clt_skt())) // only operator can set topic
                                update_topic(tab_msg.at(i), chnl_idx, clt_idx);                            
                            else
                                reply(COD_CHANOPRIVSNEEDED, channel + " " + ERR_CHANOPRIVSNEEDED, clt_idx);                            
                        }
                    }                        
                    else
                        reply(COD_INPUTTOOLONG, ERR_INPUTTOOLONG, clt_idx);        
                }
            }
            else
                reply(COD_NOTONCHANNEL, channel + " " + ERR_NOTONCHANNEL, clt_idx);
        }    
        else
            reply(COD_NOSUCHCHANNEL, channel + " " + ERR_NOSUCHCHANNEL, clt_idx);
    }    
}

void Server::update_topic(std::string topic, int chnl_idx, int clt_idx)
{    
    std::string msg_replied;
    
    if (topic == ":")
        topic = "";
    // else if (!topic.empty() && topic.at(0) == ':')
    //     topic = topic.substr(1);
    if (!topic.empty())
        topic = " " + topic;
        
    _chnls.at(chnl_idx).set_topic(topic);
    _chnls.at(chnl_idx).set_setby(_clts.at(clt_idx).get_nickname());
    _chnls.at(chnl_idx).set_setat();    
    msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
                + "@" + _clts.at(clt_idx).get_hostname() + " TOPIC " + _chnls.at(chnl_idx).get_name() \
                + topic;
    reply_to_all(msg_replied, chnl_idx);
}