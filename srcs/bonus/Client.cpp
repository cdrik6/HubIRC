/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 00:32:58 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/18 04:54:12 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(char *port)
{
	_clt_skt = create_clt_skt(port); 	
	std::cout << "Client constructed on socket " << _clt_skt << std::endl;	
	// add_pfds(&_pfds, _clt_skt, POLLIN); // client en 0
	_pfd.fd = _clt_skt;
	_pfd.events = POLLIN;  // POLLIN = Tell me when ready to read
	_pfd.revents = 0;	
}

Client::Client()
{
		
}

Client::~Client()
{
	/***** draft****** */
	std::cout << "Client destructor called\n";
	close (_clt_skt); 	
	/************** */
}

int Client::get_clt_skt(void) const
{
	return (_clt_skt);
}

std::string Client::get_msg(void) const
{
	return (_msg);
}

void Client::set_msg(std::string buffer)
{
    _msg = _msg + buffer;
}

//
int	Client::create_clt_skt(char *port)
{
	int				clt_skt;
	struct addrinfo	hints;
	struct addrinfo	*res;
	struct addrinfo	*p;
	int 			status;
	int				yes = 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;	
	// addrinfo
	status = getaddrinfo("127.0.0.1", port, &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		exit (KO); // res not allocated in case of error, no need to free
	}
	// socket + connect
	for (p = res; p; p = (*res).ai_next)
	{
		clt_skt = socket((*p).ai_family, (*p).ai_socktype, (*p).ai_protocol);
		if (clt_skt < 0)
			continue;
		if (connect(clt_skt, (*p).ai_addr, (*p).ai_addrlen) == -1)
		{
			close(clt_skt);
			perror("connect");
			continue;
		}
		break;
	}
	// freeaddrinfo(res);	
	if (p == NULL)
	{
		std::cerr << "Client socket setting failed!\n";
		exit (KO);
	}
	//
    char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string	
	char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string   
    if ((*p).ai_family == AF_INET) // IPv4
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)((*p).ai_addr);
        if (!inet_ntop(AF_INET, &((*ipv4).sin_addr), ip4, INET_ADDRSTRLEN))			
			throw (std::runtime_error("inet_ntop (ipv4): " + std::string(strerror(errno))));	
		std::cout << "New connection from " << ip4 << " on socket " << clt_skt << std::endl;
    }
    else
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)((*p).ai_addr);
        if (!inet_ntop(AF_INET6, &((*ipv6).sin6_addr), ip6, INET6_ADDRSTRLEN))
			throw (std::runtime_error("inet_ntop (ipv6): " + std::string(strerror(errno))));	
		std::cout << "New connection from " << ip6 << " on socket " << clt_skt << std::endl;
	}    
	freeaddrinfo(res);    						
	return (clt_skt);
}

void Client::polling(void)
{	
	while (_signal == false)
	{
		if (_pfd.revents & (POLLIN | POLLHUP)) 
		{
			char buff[BUFFER_SIZE + 1] = {0}; 
			int nbytes = recv(_pfd.fd, buff, BUFFER_SIZE, 0);			
			// output debug
			std::cout << "Received(" << _pfd.fd << ") [" << buff << "]\n" << std::endl;
			std::cout << "(nbytes = " << nbytes << ")\n" << std::endl;				
			//
			if (nbytes <= 0) // closed or issues
			{
				if (nbytes == 0)						
					server_disconnect();
				if (nbytes == -1)
					throw (std::runtime_error("recv: " + std::string(strerror(errno))));
			}
			else // got some data from server
			{				
				parse_message(std::string(buff));								
			}
		}		
	} // loop	
} // close fds in destructor here

// RFC 2812: message = [ ":" prefix SPACE ] command [ params ] CRLF
int Client::parse_message(std::string buffer)
{	
	set_msg(buffer);	// build message	
	if (buffer.find("\r\n") != std::string::npos)
	{		
		std::cout << "msg = [" << _msg << "]" << std::endl;			
	}	
	_msg.clear();
	return (OK);	
}

// std::cout << "Socket " << _clients.at(clt_idx).get_clt_skt() << " closed the connection\n";	
void Client::server_disconnect(void)
{	
	std::cout << "Socket " << _pfd.fd << " disconnected\n";	
	close(_pfd.fd);	
}

void Client::add_pfds(std::vector<struct pollfd>* pfds, int fd, short events)
{
	struct pollfd new_skt;
	
	new_skt.fd = fd;
	new_skt.events = events;  // POLLIN = Tell me when ready to read
	new_skt.revents = 0;
	(*pfds).push_back(new_skt);	
}
// struct pollfd {
//     int fd;         // the socket descriptor
//     short events;   // bitmap of events we're interested in
//     short revents;  // when poll() returns, bitmap of events that occurred
// };
// events :
// POLLIN 	Alert me when data is ready to recv() on this socket.
// POLLOUT 	Alert me when I can send() data to this socket without blocking.
// POLLHUP 	Alert me when the remote closed the connection.

bool Client::_signal = false; // Definition and initialization here for a static

void Client::handle_signal(int signal)
{
    std::cout << "\nSignal " << signal << " received" << std::endl;
    _signal = true;
}
/**
 * SIGINT = ctrl+c
 * SIGQUIT = ctrl+\
 * ctrl+d = not a signal
**/