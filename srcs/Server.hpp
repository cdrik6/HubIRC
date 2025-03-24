/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:29:57 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/24 03:11:15 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <sstream>
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

# define OK 0
# define KO 1

// # define PORT "6667"
# define BACKLOG 10
# define BUFFER_SIZE 1024 //512 //510


class Server
{
	private:
		int _srv_skt;		
		std::string _password;
		std::vector<Client> _clients;
		std::vector<struct pollfd> _pfds;
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
        //
		int	create_srv_skt(char *port);
		void polling(void);		
		void client_connect(void);
		void add_pfds(std::vector<struct pollfd>& pfds, int fd, short events);
		std::string printable_ip(struct sockaddr_storage client_addr, int clt_skt);
		void add_clients(std::vector<Client>& clients, int clt_skt, std::string ip);
		int client_idx(int clt_skt);
		// void build_message(std::string buffer, int clt_skt);		
		void parse_message(std::string buffer, int clt_skt);
		int  check_pass(std::vector<std::string> tab_msg, int client_idx);
		void get_command(std::vector<std::string>& tab_msg, std::string& cmd, int client_idx);
		void reply(std::string code, std::string msg_replied, int client_idx);
		// commands
		void nickname(std::vector<std::string>& tab_msg, int client_idx);
		int check_nick(std::string nick);
		int nick_available(std::string nick);
		//
		static void	handle_signal(int signal);
};

#endif

// tools
int	check_port(char* port);
std::vector<std::string> split(std::string str);
std::string toUpper(const std::string& str);

/** draft **/
// class InitException : public std::exception
// {
//     public :
//         virtual const char* what() const throw();                             
// };