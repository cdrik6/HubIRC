/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:29:57 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/18 04:53:32 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>
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

# define OK 0
# define KO 1

# define BUFFER_SIZE 1024 //512 //510


class Client
{
	private:
		int _clt_skt;			
		// std::vector<struct pollfd> _pfds;
		struct pollfd _pfd;
		std::string _msg;
		//	
		static bool _signal; // static makes _signal shared across all instances
		//
		Client(const Client& other);
	    Client& operator=(const Client& other);
		
	public:				
		Client();
		~Client();
		Client(char *port);		
		//
		int get_clt_skt(void) const;		
		std::string get_msg(void) const;
		void set_msg(std::string buffer);		
		static void	handle_signal(int signal);		
        // polling
		int	create_clt_skt(char *port);		
		void polling(void);	
		int parse_message(std::string buffer);		
		void server_disconnect(void);
		void add_pfds(std::vector<struct pollfd>* pfds, int fd, short events);
};

#endif

// tools
int	check_port(char* port);
