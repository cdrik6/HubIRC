/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:55:26 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/25 00:50:50 by caguillo         ###   ########.fr       */
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
// PASS
# define COD_PASSWDMISMATCH "464"
# define ERR_PASSWDMISMATCH ":Password incorrect"
# define ERR_PASSWDMISSING ":Password missing"
# define ERR_PASSWDREQUIRED ":Password required"
// NICK
# define RPL_NICK "NICK"
# define COD_NONICKNAMEGIVEN "431"
# define ERR_NONICKNAMEGIVEN ":No nickname given"
# define COD_ERRONEUSNICKNAME "432"
# define ERR_ERRONEUSNICKNAME ":Erroneous nickname"
# define COD_NICKNAMEINUSE "433"
# define ERR_NICKNAMEINUSE ":Nickname is already in use"
// REGISTER
# define COD_NOTREGISTERED "451"
# define ERR_NOTREGISTERED ":You have not registered"
// WELCOME
# define COD_WELCOME "001"
# define RPL_WELCOME ":Welcome to the Internet Relay Network" // <nick>!<user>@<host>
# define COD_YOURHOST "002"
# define RPL_YOURHOST ":Your host is localhost, running version causette-0.1"
# define COD_CREATED "003"
# define RPL_CREATED ":This server was created over March 2025"
# define COD_MYINFO "004"
# define RPL_MYINFO ":ircserv 0.1" // <available user modes> <available channel modes>
// MOTD
# define COD_MOTDSTART "375"
# define RPL_MOTDSTART ":- ircserv Message of the day - "
# define COD_MOTD "372"
# define RPL_MOTD ":- de do do do, de da da da"
# define COD_ENDOFMOTD "376"
# define RPL_ENDOFMOTD ":End of MOTD command"
// USER
# define COD_NEEDMOREPARAMS "461"
# define ERR_NEEDMOREPARAMS ":Not enough parameters" //<command>
# define COD_ALREADYREGISTRED "462"
# define ERR_ALREADYREGISTRED ":Unauthorized command (already registered)"


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