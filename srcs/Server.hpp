/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:29:57 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/22 01:22:00 by caguillo         ###   ########.fr       */
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

# include "Client.hpp"

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
		void build_message(std::string buffer, int clt_skt);
		void parse_message(std::string buffer, int clt_skt);
		void check_command(std::string cmd, int client_idx);
		void reply(std::string RPL_ERR, int client_idx);
		// commands
		void nickname(int client_idx);
		//
		static void	handle_signal(int signal);
};

#endif

// tools
int	check_port(char* port);
std::vector<std::string> split(std::string str);

/** draft **/
// class InitException : public std::exception
// {
//     public :
//         virtual const char* what() const throw();                             
// };