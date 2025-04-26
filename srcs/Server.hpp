/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:29:57 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/27 01:25:10 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <set>
# include <sstream>
# include <algorithm>
# include <string>
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
# include <ctime>

# include "Client.hpp"
# include "Register.hpp"
# include "Channel.hpp"

# define OK 0
# define KO 1

# define BACKLOG 10 // pending connections before the kernel starts rejecting new ones
# define BUFFER_SIZE 1024 //512 //510
# define MAX_CHANNEL_LIMIT 1000
# define PASSLEN 31
# define NICKLEN 30
# define TOPICLEN 307
# define KICKLEN 307
# define CHANLEN 50
# define USERLEN 18
# define NAMEOPER "admin" // to get OPER usable
# define PASSOPER "1234" // to get OPER usable
# define BOTKEY "robot"


class Channel;

class Server
{
	private:
		int _srv_skt;		
		std::string _password;
		std::vector<Client> _clts;
		std::vector<struct pollfd> _pfds;
		std::vector<Channel> _chnls;
		std::vector<int> _fails;
		std::set<int> _replied_clts;
		//		
		static bool _signal; // static makes _signal shared across all instances
		//
		Server(const Server& other);
	    Server& operator=(const Server& other);	
			
	public:				
		Server();
		~Server();
		Server(char *port, std::string password);		
		//
		int get_srv_skt(void) const;		
		static void handle_signal(int signal);		
        // polling
		int	create_srv_skt(char *port);
		void polling(void);		
		void client_connect(void);
		void client_disconnect(std::string reason, int pfd_idx, int clt_idx);		
		void add_pfds(int fd, short events);
		std::string printable_ip(struct sockaddr_storage client_addr, int clt_skt);
		void add_clients(int clt_skt, std::string ip);
		int client_idx(int clt_skt);
		void clean_fails(void);
		// get command
		int parse_message(std::string buffer, int clt_idx);
		int check_registered(int clt_idx);		
		int get_command(std::vector<std::string>& tab_msg, std::string& cmd, int clt_idx, size_t tab_idx);		
		// Register
		void reply(std::string code, std::string msg_replied, int clt_idx);		
		void ping(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		void pass(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		void nickname(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);		
		int check_nick(std::string nick);
		int nick_available(std::string nick, int clt_idx);
		void username(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		int check_user(std::string user);
		void welcome(int clt_idx);		
		// Privmsg
		void privmsg(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		int in_channel(int chnl_idx, int clt_idx);
		int target_clt_idx(std::string target);
		int target_clt_fd(std::string target);
		int target_chnlclt_idx(std::string target, int chnl_idx);
		int target_chnlclt_fd(std::string target, int chnl_idx);
		int channel_idx(std::string channel);
		// Join
		void join(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		void create_chnl(std::string name, std::string key, int clt_idx);
		void reply_join_add(std::string channel, int chnl_idx, int clt_idx);
		void reply_join_new(std::string channel, int clt_idx);
		int check_channel(std::string chan_name);
		int check_key(std::string key_name);		
		// Who
		void who(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);				
		// Part
		void part(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		void reply_to_all(std::string msg_replied, int chnl_idx);
		// Mode
		void mode(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		std::string get_modes(int chnl_idx, int clt_idx);		
		void parse_mode(std::string modestr, std::vector<std::string> *modes, std::vector<std::string> *args, bool first);
		std::vector<std::string> set_plus(std::string plus, std::vector<std::string>& params, int chnl_idx, int clt_idx);
		std::vector<std::string> set_minus(std::string minus, std::vector<std::string>& params, int chnl_idx, int clt_idx);
		int nick_in_channel(int chnl_idx, std::string nick);
		int check_limit(std::string limit);
		// Topic
		void topic(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		void update_topic(std::string topic, int chnl_idx, int clt_idx);
		// Kick
		void kick(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		void kick_users(std::vector<std::string> users, std::string reason, int chnl_idx, int clt_idx);
		// Invite
		void invite(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		void let_it_in(int chnl_idx, int clt_idx, int inv_idx);
		// Quit
		void quit(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		void quit_channels(std::string reason, int clt_idx);
		void rem_empty_chnl(void);
		// Notice
		void notice(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		// Oper
		void oper(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
		// IamBot
		void iambot(std::vector<std::string>& tab_msg, int clt_idx, size_t tab_idx);
};

#endif

// tools
int	check_port(char* port);
std::vector<std::string> split(std::string str);
std::vector<std::string> split_char(std::string str, char c);
std::string toUpper(const std::string& str);
