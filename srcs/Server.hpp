/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:29:57 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/10 01:42:32 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
// # include <sstream>
//
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <poll.h>
//
# include <cerrno>
# include <cstdlib>
// #include <string.h>
#include <cstdio>
// #include <unistd.h>

# define OK 0
# define KO 1

// # define PORT "6667"
# define BACKLOG 10


class Server
{
	private:
		int _srv_skt;
		Server();

	public:		
		Server(char *port, std::string password);
		Server& operator=(const Server& other);
        Server(const Server& other);        
		~Server();
		//
		int get_srv_skt(void) const;		
        //
		int	init_srv_skt(char *port, std::string password);	
		//
		// class InitException : public std::exception
        // {
        //     public :
        //         virtual const char* what() const throw();                             
        // };    
};

#endif