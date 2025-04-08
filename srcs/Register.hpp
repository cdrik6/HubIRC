/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Register.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:55:26 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/08 18:58:41 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REGISTER_HPP
# define REGISTER_HPP

# include "Server.hpp"
// RFC 2812: message    =  [ ":" prefix SPACE ] command [ params ] CRLF
// RPL Format ":<server_hostname> <code> <nickname> :<message>" CRLF
// RPL Format ":server_name <numeric_reply> <target_nick> <parameters> :<message>" CRLF

// Replies
// Numerics in the range from 001 to 099 are used for client-server connections only
// Replies generated in the response to commands are found in the range from 200 to 399
// Error replies are found in the range from 400 to 599

// /server ADD myserver 127.0.0.1 6667 mypassword
// Edit ~/.irssi/config

# define COD_NONE "000"
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
# define COD_NORECIPIENT "411"
# define ERR_NORECIPIENT ":No recipient given (PRIVMSG)"
# define COD_NOTEXTTOSEND "412"
# define ERR_NOTEXTTOSEND ":No text to send"
# define COD_NOSUCHNICK "401"
# define ERR_NOSUCHNICK ":No such nick/channel" //"<nickname> :No such nick/channel"
# define COD_NOSUCHCHANNEL "403"
# define ERR_NOSUCHCHANNEL ":No such channel" //"<channel> :No such channel"
# define COD_CANNOTSENDTOCHAN "404"
# define ERR_CANNOTSENDTOCHAN ":Cannot send to channel"
// JOIN
# define COD_BADCHANNELKEY "475" 
# define ERR_BADCHANNELKEY ":wrong key to join the channel" // "<client> <channel> :Cannot join channel (+k)"
# define COD_TOPIC "332" // 332 <nickname> <channel> :<topic>
# define COD_NOTOPIC "331" // 331 <nickname> <channel> :No topic is set
# define RPL_NOTOPIC ":No topic set yet"
# define COD_NAMREPLY "353"
# define COD_ENDOFNAMES "366"
# define RPL_ENDOFNAMES ":End of /NAMES list"
# define COD_USERONCHANNEL "443"
# define ERR_USERONCHANNEL ":is already on channel"  //"<client> <nick> <channel> :is already on channel"
# define COD_CHANNELISFULL "471"
# define ERR_CHANNELISFULL ":Cannot join channel (+l)" // <channel>
// WHO 
# define COD_WHOREPLY "352"
# define COD_ENDOFWHO "315"
# define RPL_ENDOFWHO ":End of /WHO list"
// PART
# define COD_NOTONCHANNEL "442" 
# define ERR_NOTONCHANNEL ":You're not on that channel"
// MODE 
# define COD_CHANNELMODEIS "324" // 329 time optional 
# define COD_CHANOPRIVSNEEDED "482" 
# define ERR_CHANOPRIVSNEEDED ":You're not channel operator"
# define COD_UNKNOWNMODE "472"
# define ERR_UNKNOWNMODE ":is unknown mode char to me" // <modechar>
# define COD_INVALIDKEY "525"
# define ERR_INVALIDKEY ":Key is not well-formed" // <target chan>
# define COD_USERNOTINCHANNEL "441"
# define ERR_USERNOTINCHANNEL ":They aren't on that channel" // <nick> <channel>
# define COD_UNKNOWNERROR "400"
# define ERR_UNKNOWNERROR ":Invalid limit" //<command>{ <subcommand>} :<info>" for limit > MAX
// TOPIC
# define COD_TOPICWHOTIME "333"
//# define RPL_TOPICWHOTIME "" // <channel> <nick> <setat>


"<client> <channel> <nick> <setat>"


// // COMMAND NOT FOUND
// # define COD_UNKNOWNCOMMAND "421"
// # define ERR_UNKNOWNCOMMAND " :Unknown command" // "<command> :Unknown command"


#endif