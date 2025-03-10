/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:29:57 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/11 00:52:17 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
// # include <sstream>
//
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/poll.h>
# include <fcntl.h>
//
# include <cerrno>
# include <cstdlib>
# include <cstring>
# include <cstdio>
// #include <unistd.h>

# define OK 0
# define KO 1

// # define PORT "6667"
# define BACKLOG 10


class Server
{
	private:
		int _srv_skt;		
		std::string _password;
		std::vector<struct pollfd> _pfds;
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
		
		// class InitException : public std::exception
        // {
        //     public :
        //         virtual const char* what() const throw();                             
        // };    
};

#endif