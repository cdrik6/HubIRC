/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 00:32:58 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/10 01:50:30 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *port, std::string password)
{
	// try
	// {
	_srv_skt = init_srv_skt(port, password);
	std::cout << "Server constructed on socket: " << _srv_skt << std::endl;
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << std::endl;
	// }	
}

Server& Server::operator=(const Server& other)
{
	// port and password *************************************************/
	std::cout << "Server copy assignment operator called" << std::endl;
    if (this == &other)
        return (*this);
	_srv_skt = other._srv_skt;    
    return (*this);
}

Server::Server(const Server& other)
{
	std::cout << "Server copy constructor of socket: " << other._srv_skt << std::endl;
    *this = other;
}        

Server::~Server()
{
	
}

//
int Server::get_srv_skt(void) const
{
	return (_srv_skt);
}

//
int	Server::init_srv_skt(char *port, std::string password)
{
	int				srv_skt;
	struct addrinfo	hints;
	struct addrinfo	*res;
	struct addrinfo	*p;
	int 			status;
	int				yes = 1;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// addrinfo
	if (status = getaddrinfo(NULL, port, &hints, &res) != 0)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		exit(KO); // res not allocated in case of error, no need to free
	}
	// socket + allowing reuse a port + bind
	for (p = res; p; p = (*res).ai_next)
	{
		srv_skt = socket((*p).ai_family, (*p).ai_socktype, (*p).ai_protocol);
		if (srv_skt < 0)
			continue;
		if (setsockopt(srv_skt, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			perror("setsockopt");
			freeaddrinfo(res);
			exit(KO);
		}			
		if (bind(srv_skt, (*p).ai_addr, (*p).ai_addrlen) < 0)
			continue;
		break;
	}
	freeaddrinfo(res);
	// can't 
	if (p == NULL)
	{
		std::cerr << "Server socket setting failed!\n";
		exit(KO);
	}		
	// listen
	if (listen(srv_skt, BACKLOG) == -1)
	{
		perror("listen");
		exit (KO);
	}
		
		
	//	
	printf("server: waiting for connections...\n");
	//	
	return (srv_skt);	
}	

// throw(Bureaucrat::GradeTooHighException()); /*************** */
// exception
// const char* Server::InitException::what() const throw()
// {
//     return (" ................ ");    
// }