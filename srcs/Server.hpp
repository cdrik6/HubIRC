/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:29:57 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/02 23:35:35 by caguillo         ###   ########.fr       */
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
# include "Command.hpp"
# include "Channel.hpp"

# define OK 0
# define KO 1

// # define PORT "6667"
# define BACKLOG 10
# define BUFFER_SIZE 1024 //512 //510

class Channel;

class Server
{
	private:
		int _srv_skt;		
		std::string _password;
		std::vector<Client> _clients;
		std::vector<struct pollfd> _pfds;
		std::vector<Channel> _chnls;
		//
		static bool _signal; //static makes _signal shared across all instances
		//
		Server();
		Server& operator=(const Server& other);
        Server(const Server& other);
		
	public:		
		Server(char *port, std::string password);		
		~Server();
		//
		int get_srv_skt(void) const;		
        // polling
		int	create_srv_skt(char *port);
		void polling(void);		
		void client_connect(void);
		void client_disconnect(int pfd_idx, int clt_idx);
		void quit_channels(std::string reason, int clt_idx);
		void add_pfds(std::vector<struct pollfd>& pfds, int fd, short events);
		std::string printable_ip(struct sockaddr_storage client_addr, int clt_skt);
		void add_clients(std::vector<Client>& clients, int clt_skt, std::string ip);
		int client_idx(int clt_skt);
		// get command
		int parse_message(std::string buffer, int clt_idx);
		int check_registered(int clt_idx);
		void welcome(int clt_idx);		
		void get_command(std::vector<std::string>& tab_msg, std::string& cmd, int clt_idx);		
		// commands
		void reply(std::string code, std::string msg_replied, int clt_idx);		
		void ping(int clt_idx);
		void pass(std::vector<std::string>& tab_msg, int clt_idx);
		void nickname(std::vector<std::string>& tab_msg, int clt_idx);
		void username(std::vector<std::string>& tab_msg, int clt_idx);
		int check_nick(std::string nick);
		int nick_available(std::string nick, int clt_idx);
		void privmsg(std::vector<std::string>& tab_msg, int clt_idx);
		int target_index(std::string target);
		void join(std::vector<std::string>& tab_msg, int clt_idx);
		void add_chnls(std::vector<Channel>& chans, std::string name, std::string key, int clt_idx);
		void reply_join_add(std::string channel, int chnl_idx, int clt_idx);
		void reply_join_new(std::string channel, int clt_idx);
		int check_channel(std::string chan_name);
		int check_key(std::string key_name);
		//
		static void	handle_signal(int signal);
		//void ping(std::vector<std::string>& tab_msg, int clt_idx);
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

/** draft **/
// class InitException : public std::exception
// {
//     public :
//         virtual const char* what() const throw();                             
// };