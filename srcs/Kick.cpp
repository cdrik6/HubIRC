/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 23:07:33 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/10 04:13:53 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

// <channel> <user> *( "," <user> ) [:<comment>]
void Server::kick(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx)
{
    int i = tab_idx + 1;
    
    if (i >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "KICK " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
    else if (tab_msg.at(i).at(0) != '#' && tab_msg.at(i).at(0) != '&')
        reply(COD_NOSUCHCHANNEL, tab_msg.at(i) + " " + ERR_NOSUCHCHANNEL, clt_idx);    
    else	
    {
		std::string channel = tab_msg.at(i);
		int chnl_idx = channel_idx(channel);
		
		if (chnl_idx != -1)
        {			
			if (in_channel(chnl_idx, clt_idx) != -1) // kicker on channel?
			{
				if (_chnls.at(chnl_idx).is_operator(_clts.at(clt_idx).get_nickname())) // kicker operator?
				{
					i++;		
					if (i >= tab_msg.size())
						reply(COD_NEEDMOREPARAMS, "KICK " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
					else
					{					
						std::vector<std::string> users = split_char(tab_msg.at(i), ',');
						std::string reason = DEFAULT_REASON;						
						
						if (i + 1 < tab_msg.size())
							reason = tab_msg.at(i + 1);						
						if (users.size() == 0)
							reply(COD_NEEDMOREPARAMS, "KICK " + std::string(ERR_NEEDMOREPARAMS), clt_idx);
						else
							kick_users(users, reason, chnl_idx, clt_idx);
					}
				}                
				else
					reply(COD_CHANOPRIVSNEEDED, channel + " " + ERR_CHANOPRIVSNEEDED, clt_idx);                      		
			}
			else
				reply(COD_NOTONCHANNEL, channel + " " + ERR_NOTONCHANNEL, clt_idx);
		}
		else
            reply(COD_NOSUCHCHANNEL, channel + " " + ERR_NOSUCHCHANNEL, clt_idx);
	}	
}

// :<kicker>!user@host KICK <channel> <target> :<reason>
void Server::kick_users(std::vector<std::string> users, std::string reason, int chnl_idx, int clt_idx)
{
	std::string msg_replied;
	msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!~" + _clts.at(clt_idx).get_username() \
                + "@" + _clts.at(clt_idx).get_hostname() + " KICK " + _chnls.at(chnl_idx).get_name();                
	
	for (int j = 0; j < users.size(); j++)
	{
		int tgt_idx = target_idx(users.at(j));
		
		if (tgt_idx != -1)
		{
			if (in_channel(chnl_idx, tgt_idx) != -1) // user kicked on channel?
			{
				msg_replied = msg_replied + " " + users.at(j) + " " + reason;
				reply_to_all(msg_replied, chnl_idx);
				_chnls.at(chnl_idx).rem_client(tgt_idx);
				_chnls.at(chnl_idx).rem_operator(_clts.at(tgt_idx).get_nickname());
                _chnls.at(chnl_idx).rem_invitee(_clts.at(tgt_idx).get_nickname());
			}
			else
				reply(COD_USERNOTINCHANNEL, users.at(j) + " " + _chnls.at(chnl_idx).get_name() + " " + ERR_USERNOTINCHANNEL, clt_idx);
		}
		else
			reply(COD_USERNOTINCHANNEL, users.at(j) + " " + _chnls.at(chnl_idx).get_name() + " " + ERR_USERNOTINCHANNEL, clt_idx);		
	}	
}