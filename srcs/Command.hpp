/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:55:26 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/29 19:59:35 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP


# include "Server.hpp"
// RFC 2812: message    =  [ ":" prefix SPACE ] command [ params ] CRLF
// RPL Format ":<server_hostname> <code> <nickname> :<message>" CRLF
// RPL Format ":server_name <numeric_reply> <target_nick> <parameters> :<message>" CRLF

// Replies
// Numerics in the range from 001 to 099 are used for client-server connections only
// Replies generated in the response to commands are found in the range from 200 to 399
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
# define COD_NICKNAMEINUSE "433" // ERR_NICKNAMEINUSE ":Nickname is already in use"
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
// PRIVMSG
# define COD_AWAY "301" // RPL_AWAY = "<nick> :<away message>"
# define COD_NORECIPIENT "411"
# define ERR_NORECIPIENT ":No recipient given (PRIVMSG)"
# define COD_NOTEXTTOSEND "412"
# define ERR_NOTEXTTOSEND ":No text to send"
# define COD_NOSUCHNICK "401"
# define ERR_NOSUCHNICK " :No such nick/channel" //"<nickname> :No such nick/channel"

// // COMMAND NOT FOUND
// # define COD_UNKNOWNCOMMAND "421"
// # define ERR_UNKNOWNCOMMAND " :Unknown command" // "<command> :Unknown command"


#endif