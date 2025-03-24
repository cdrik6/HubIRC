/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:55:26 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/24 03:19:44 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP


# include "Server.hpp"

# define CRLF "\r\n"
# define NOCODE ""

// RFC 2812: message    =  [ ":" prefix SPACE ] command [ params ] CRLF
// RPL Format ":<server_hostname> <code> <nickname> :<message>" CRLF
// RPL Format ":server_name <numeric_reply> <target_nick> <parameters> :<message>" CRLF

// Replies
// Numerics in the range from 001 to 099 are used for client-server connections only
// Replies generated in the response to commands are found in the range from 200 to 399
// Nick


// # define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)
// # define RPL_UMODEIS(hostname, channelname, mode, user)  ":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF
// # define RPL_CREATIONTIME(nickname, channelname, creationtime) ": 329 " + nickname + " #" + channelname + " " + creationtime + CRLF
// # define RPL_CHANNELMODES(nickname, channelname, modes) ": 324 " + nickname + " #" + channelname + " " + modes + CRLF
// # define RPL_CHANGEMODE(hostname, channelname, mode, arguments) (":" + hostname + " MODE #" + channelname + " " + mode + " " + arguments + CRLF)

// # define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN #" + channelname + CRLF)
// # define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ #" + channelname + " :" + clientslist + CRLF)
// # define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " #" + channelname + " :END of /NAMES list" + CRLF)
// # define RPL_TOPICIS(nickname, channelname, topic) (": 332 " + nickname + " #" +channelname + " :" + topic + "\r\n")

// Error replies are found in the range from 400 to 599
//PASS
# define ERR_PASSWDMISMATCH "464"
# define RPL_PASSWDMISMATCH "Unvalid password"
# define RPL_PASSWDREQUIRED "Password required"
# define RPL_PASSWDMISSING "Password missing"
//NICK
# define RPL_NICK "NICK"
# define ERR_NONICKNAMEGIVEN "431"
# define RPL_NONICKNAMEGIVEN "No nickname given"
# define ERR_ERRONEUSNICKNAME "432"
# define RPL_ERRONEUSNICKNAME "Erroneous nickname"
# define ERR_NICKNAMEINUSE "433"
# define RPL_NICKNAMEINUSE "Nickname is already in use"

//
// # define ERR_NEEDMODEPARM(channelname, mode) (": 696 #" + channelname + " * You must specify a parameter for the key mode. " + mode + CRLF)
// # define ERR_INVALIDMODEPARM(channelname, mode) ": 696 #" + channelname + " Invalid mode parameter. " + mode + CRLF
// # define ERR_KEYSET(channelname) ": 467 #" + channelname + " Channel key already set. " + CRLF
// # define ERR_UNKNOWNMODE(nickname, channelname, mode) ": 472 " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + CRLF
// # define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + CRLF)
// # define ERR_CHANNELNOTFOUND(nickname, channelname) (": 403 " + nickname + " " + channelname + " :No such channel" + CRLF)
// # define ERR_NOTOPERATOR(channelname) (": 482 #" + channelname + " :You're not a channel operator" + CRLF)
// # define ERR_NOSUCHNICK(channelname, name) (": 401 #" + channelname + " " + name + " :No such nick/channel" + CRLF )
// # define ERR_INCORPASS(nickname) (": 464 " + nickname + " :Password incorrect !" + CRLF )
// # define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + CRLF )
// # define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + CRLF)
// # define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + CRLF)



#endif