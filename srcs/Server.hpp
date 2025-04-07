/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:29:57 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/08 00:56:47 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <sstream>
# include <algorithm>
//
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/poll.h>
# include <fcntl.h>
# include <unistd.h>
//
# include <cerrno>
# include <cstdlib>
# include <cstring>
# include <cstdio>
# include <csignal>
# include <cctype>

# include "Client.hpp"
# include "Register.hpp"
# include "Channel.hpp"

# define OK 0
# define KO 1

# define BACKLOG 10 // pending connections before the kernel starts rejecting new ones
# define BUFFER_SIZE 1024 //512 //510
# define MAX_CHANNEL_LIMIT 1000


class Channel;

class Server
{
	private:
		int _srv_skt;		
		std::string _password;
		std::vector<Client> _clts;
		std::vector<struct pollfd> _pfds;
		std::vector<Channel> _chnls;
		//
		static bool _signal; // static makes _signal shared across all instances
		//
		Server();
		Server& operator=(const Server& other);
        Server(const Server& other);
		
	public:		
		Server(char *port, std::string password);		
		~Server();
		//
		int get_srv_skt(void) const;		
		static void	handle_signal(int signal);
		void rem_empty_chnl(void);
        // polling
		int	create_srv_skt(char *port);
		void polling(void);		
		void client_connect(void);
		void client_disconnect(int pfd_idx, int clt_idx);
		void quit_channels(std::string reason, int clt_idx);
		void add_pfds(std::vector<struct pollfd>& pfds, int fd, short events);
		std::string printable_ip(struct sockaddr_storage client_addr, int clt_skt);
		void add_clients(std::vector<Client>& clts, int clt_skt, std::string ip);
		int client_idx(int clt_skt);		
		// get command
		int parse_message(std::string buffer, int clt_idx);
		int check_registered(int clt_idx);
		void welcome(int clt_idx);		
		void get_command(std::vector<std::string>& tab_msg, std::string& cmd, int clt_idx, int tab_idx);		
		// Register
		void reply(std::string code, std::string msg_replied, int clt_idx);		
		void ping(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);
		void pass(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);
		void nickname(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);
		void username(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);
		int check_nick(std::string nick);
		int nick_available(std::string nick, int clt_idx);
		// Privmsg
		void privmsg(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);
		int in_channel(int chnl_idx, int clt_idx);
		int target_index(std::string target);
		int channel_idx(std::string channel);
		// Join
		void join(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);
		void add_chnls(std::vector<Channel>& chans, std::string name, std::string key, int clt_idx);
		void reply_join_add(std::string channel, int chnl_idx, int clt_idx);
		void reply_join_new(std::string channel, int clt_idx);
		int check_channel(std::string chan_name);
		int check_key(std::string key_name);
		// int check_already(int chnl_idx, int clt_idx);
		// Who
		void who(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);				
		// Part
		void part(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);
		void reply_to_all(std::string msg_replied, int chnl_idx);
		// Mode
		void mode(std::vector<std::string>& tab_msg, int clt_idx, int tab_idx);
		std::string get_modes(int chnl_idx, int clt_idx);
		std::string get_plus(std::string modestr);
		std::string get_minus(std::string modestr);
		void set_plus(std::string plus, std::vector<std::string>& params, int chnl_idx, int clt_idx);
		void set_minus(std::string plus, std::vector<std::string>& params, int chnl_idx, int clt_idx);
		int nick_in_channel(int chnl_idx, std::string nick);
		long check_limit(std::string limit);
		// void ping(std::vector<std::string>& tab_msg, int clt_idx);
		// void build_message(std::string buffer, int clt_skt);		
		// int check_pass(std::vector<std::string>& tab_msg, int client_idx);
		// void cap(std::vector<std::string>& tab_msg, int client_idx);
};

#endif

// tools
int	check_port(char* port);
std::vector<std::string> split(std::string str);
std::vector<std::string> split_char(std::string str, char c);
std::string toUpper(const std::string& str);
