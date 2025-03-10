/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 00:32:58 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/11 00:56:17 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *port, std::string password)
{
	_password = password;
	_srv_skt = create_srv_skt(port);
	std::cout << "Server constructed on socket: " << _srv_skt << std::endl;
	_pfds[0].fd = _srv_skt;
	_pfds[0].events = POLLIN; // Tell me when ready to read
	std::cout << "Server: waiting for connections...\n";		
}

// Server& Server::operator=(const Server& other)
// {
// 	// port and password *************************************************/
// 	std::cout << "Server copy assignment operator called" << std::endl;
//     if (this == &other)
//         return (*this);
// 	_srv_skt = other._srv_skt;    
//     return (*this);
// }

// Server::Server(const Server& other)
// {
// 	std::cout << "Server copy constructor of socket: " << other._srv_skt << std::endl;
//     *this = other;
// }        

Server::~Server()
{
	
}

//
int Server::get_srv_skt(void) const
{
	return (_srv_skt);
}

//
int	Server::create_srv_skt(char *port)
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
		exit (KO); // res not allocated in case of error, no need to free
	}
	// socket + allowing reuse a port + non-blocking + bind
	for (p = res; p; p = (*res).ai_next)
	{
		srv_skt = socket((*p).ai_family, (*p).ai_socktype, (*p).ai_protocol);
		if (srv_skt < 0)
			continue;
		if (setsockopt(srv_skt, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			(perror("setsockopt"), freeaddrinfo(res), exit (KO));				
		if (fcntl(_srv_skt, F_SETFL, O_NONBLOCK) == -1)		
			(perror("fcntl"), freeaddrinfo(res), exit (KO));				  
		if (bind(srv_skt, (*p).ai_addr, (*p).ai_addrlen) < 0)
			continue;
		break;
	}
	freeaddrinfo(res);	
	if (p == NULL)
	{
		std::cerr << "Server socket setting failed!\n";
		exit (KO);
	}		
	// listen
	if (listen(srv_skt, BACKLOG) == -1)
	{
		perror("listen");
		exit (KO);
	}					
	return (srv_skt);
}	

void Server::polling(void)
{
	// poll returns the number of elements in the array that have had an event occur
	int event_count = poll(_pfds.data(), _pfds.size(), -1); // -1 = wait forever
	if ( event_count == -1) 		
		throw (std::runtime_error("poll: " + std::string(strerror(errno))));
		
}


// throw(Bureaucrat::GradeTooHighException()); /*************** */
// exception
// const char* Server::InitException::what() const throw()
// {
//     return (" ................ ");    
// }