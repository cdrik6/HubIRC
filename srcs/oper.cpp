/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 03:38:36 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/24 23:23:05 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// OPER <name> <password>
void Server::oper(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx)
{       
    size_t i = tab_idx + 1;    
    
    if (i + 1 >= tab_msg.size())
        reply(COD_NEEDMOREPARAMS, "OPER " + std::string(ERR_NEEDMOREPARAMS), clt_idx);    
    else
    {        
        std::string name = tab_msg.at(i);
        std::string pass = tab_msg.at(i + 1);        
        
        if (name == NAMEOPER && pass == PASSOPER)  
            reply(COD_YOUREOPER, RPL_YOUREOPER, clt_idx);
        else
            reply(COD_PASSWDMISMATCH, ERR_PASSWDMISMATCH, clt_idx);	// anycase passmismatch to be replied for security reason
    }
}
