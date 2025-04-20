/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 23:07:33 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/20 03:53:31 by caguillo         ###   ########.fr       */
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
				if (_chnls.at(chnl_idx).is_operator(_clts.at(clt_idx).get_clt_skt())) // kicker operator?
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
						// if (!reason.empty() && reason.at(0) == ':')
						// 	reason = reason.substr(1);
						if (!reason.empty())
							reason = " " + reason;
						if (reason.length() > KICKLEN)
							reason = reason.substr(0, KICKLEN);
													
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
	msg_replied = ":" + _clts.at(clt_idx).get_nickname() + "!" + _clts.at(clt_idx).get_username() \
                + "@" + _clts.at(clt_idx).get_hostname() + " KICK " + _chnls.at(chnl_idx).get_name();                
	
	for (int j = 0; j < users.size(); j++)
	{
		// int tgt_idx = target_chnlclt_idx(users.at(j), chnl_idx);		
		// std::cout << "tgt idx = " << tgt_idx << std::endl;
		// std::string tgt_nick = _clts.at(client_idx(_chnls.at(chnl_idx).get_chnlclts().at(tgt_idx))).get_nickname();		
		// std::cout << "target kicked = " << tgt_nick << std::endl;
		int tgt_fd = target_chnlclt_fd(users.at(j), chnl_idx);
		// std::cout << "tgt fd = " << tgt_fd << std::endl;
		// std::string tgt_nick = _clts.at(client_idx(tgt_fd)).get_nickname();
		// std::cout << "target kicked = " << tgt_nick << std::endl;
		// std::cout << "in_channel = " << in_channel(chnl_idx, tgt_idx) << std::endl;
		
		if (tgt_fd != -1)
		{			
			msg_replied = msg_replied + " " + users.at(j) + reason;
			reply_to_all(msg_replied, chnl_idx);			
			// if (is_operator())
			// std::cout << "je suis ici " << std::endl;
			// std::cout << "tgt_fd = "<< tgt_fd << std::endl;				
			_chnls.at(chnl_idx).rem_operator(tgt_fd);
			// std::cout << "je suis ici la" << std::endl;
			// if (is_invitee())
			_chnls.at(chnl_idx).rem_invitee(tgt_fd);
			// std::cout << "je suis encore" << std::endl;
			_chnls.at(chnl_idx).rem_chnlclt(tgt_fd);			
		}
		else
			reply(COD_USERNOTINCHANNEL, users.at(j) + " " + _chnls.at(chnl_idx).get_name() + " " + ERR_USERNOTINCHANNEL, clt_idx);		
	}	
}