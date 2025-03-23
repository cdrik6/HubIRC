/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 00:32:58 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/23 02:39:12 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Relais pour la causette Internet 
#include "Server.hpp"

Server::Server(char *port, std::string password)
{
	add_pfds(_pfds, STDIN_FILENO, POLLIN); // add std_in en 0	
	_password = password;
	_srv_skt = create_srv_skt(port); 
	std::cout << "Server constructed on socket " << _srv_skt << std::endl;	
	add_pfds(_pfds, _srv_skt, POLLIN); // server en 1	
	std::cout << "Server: waiting for connections...\n";
	std::cout << "Server: \"stop\" to stop it\n";
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
	/***** draft****** */
	std::cout << "destructor called\n";
	// close (_srv_skt); // i = 1
	for (int i = 1; i < _pfds.size(); i++)
	{
		close (_pfds.at(i).fd);
	}
	/************** */
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

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// addrinfo
	status = getaddrinfo(NULL, port, &hints, &res);
	if (status != 0)
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
		if (fcntl(srv_skt, F_SETFL, O_NONBLOCK) == -1)
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
// Note 
// SOCK_STREAM uses TCP
// when using SOCK_STREAM, set the protocol to 0, and it’ll use the proper protocol automatically

void Server::polling(void)
{	
	while (_signal == false)
	{
		int event_count = poll(_pfds.data(), _pfds.size(), -1); // -1 = wait forever

		// poll returns the number of elements in the array that have had an event occur
		if (event_count == -1 && _signal == false)
			throw (std::runtime_error("poll: " + std::string(strerror(errno))));
		if (event_count == 0)
			continue;
		
		// if STD_IN got data to read
		if (_pfds.at(0).revents & POLLIN)
		{				
			char buff[BUFFER_SIZE + 1] = {0}; //memset(buff, 0, sizeof(buff));
			int nbytes = read(STDIN_FILENO, buff, BUFFER_SIZE);
			if (nbytes == -1)
				throw (std::runtime_error("read: " + std::string(strerror(errno))));
			if (nbytes == 0 || std::string(buff, nbytes) == "stop\n")
				break;	
		}		
		
		// if server get sthg to read (--> new connection)	
		if (_pfds.at(1).revents & POLLIN)
		{
			try
			{
				client_connect();
			}
			catch (const std::exception& e) { throw; }
		}
				
		// ckeck revents of clients 
		for (int i = 2; i < _pfds.size(); i++)
		{
			if (_pfds.at(i).revents & (POLLIN | POLLHUP)) // if got one ready to read
			{
				char buff[BUFFER_SIZE + 1] = {0}; //memset(buff, 0, sizeof(buff));
				int nbytes = recv(_pfds.at(i).fd, buff, BUFFER_SIZE, 0);
				std::cout << "recvbuff: " << buff << std::endl;
				std::cout << "nbytes: " << nbytes << std::endl;
				if (nbytes <= 0) // closed or issues
				{
					if (nbytes == 0)
					{
						/*************** close everything with the client *********************/						
						std::cout << "Socket " << _pfds.at(i).fd << " closed the connection\n";
						close (_pfds.at(i).fd);					
						_pfds.erase(_pfds.begin() + i);	
						/**********************************************************************/
						// int k = client_idx(_pfds.at(i).fd);
						
					}					
					if (nbytes == -1) //***** everything MUST be closed at main level then *****/
						throw (std::runtime_error("recv: " + std::string(strerror(errno))));
				}
				else // got some data from a client --> to send the others (not srv not sender)
				{
					build_message(std::string(buff), _pfds.at(i).fd);
					parse_message(std::string(buff), _pfds.at(i).fd);
					/*******  Parsing msg received to exec CMD and build RPL to client (irssi) **/
					// for (int j = 2; j < _pfds.size(); j++)
					// {
					// 	if (j != i && send(_pfds[j].fd, buff, nbytes, MSG_NOSIGNAL) == - 1)
					// 		throw (std::runtime_error("send: " + std::string(strerror(errno))));
					// 	std::cout << "sendbuff: " << buff << std::endl;
					// }						
				}
			}
		} // clients		
	} // loop	
} // close fds in destructor here
// Note: flag in send ()
// if the socket has been closed by either side, the process calling send() will get the signal SIGPIPE.
// Unless send() was called with the MSG_NOSIGNAL flag.

void Server::build_message(std::string buffer, int clt_skt)
{
	int k = client_idx(clt_skt);
	
	if (k != -1)	
		_clients.at(k).set_msg(buffer);
}

// RFC 2812: message    =  [ ":" prefix SPACE ] command [ params ] CRLF
void Server::parse_message(std::string buffer, int clt_skt)
{	
	std::vector<std::string> tab_msg;	
	int k = client_idx(clt_skt);
	// std::cout << "client idx = " << k << std::endl;
	// std::cout << "client fd = " << _clients.at(k).get_clt_skt() << std::endl;
		
	if (k != -1 && buffer.find("\n") != std::string::npos) // \n for nc \r\n for irssi
	{	
		tab_msg = split(_clients.at(k).get_msg());
		for (int i = 0; i < tab_msg.size(); i++)
		{
			std::cout << i << " = " << tab_msg[i] << std::endl;
			check_command(tab_msg[i], k);
		}		
		_clients.at(k).clear_msg();
	}
}

void Server::check_command(std::string cmd, int k)
{
	if (cmd == "NICK" || cmd == "nick")
		nickname(k);
	// else if (cmd == "CAP" || cmd == "cap")
	//  	cap(k);	
	// else if (cmd == "PASS" || cmd == "pass")
	// 	authenticate(k);
	// else if (cmd == "USER" || cmd == "user")
	// 	username(k);
	// else if (cmd == "JOIN" || cmd == "join")
	// 	join(k);
	// else if (cmd == "PRIVMSG" || cmd == "privmsg")
	// 	privmsg(k);
	// else if (cmd == "KICK" || cmd == "kick")
	// 	kick(k);
}


int Server::client_idx(int clt_skt)
{
	int k = -1;	// disconnected in between or error ?
	
	for (int i = 0; i < _clients.size(); i++)
	{		
		if (_clients.at(i).get_clt_skt() == clt_skt)
		{	
			k = i;
			break;
		}
	}	
	return (k);
}


/******* PASSWORD Authanticate *****/
void Server::client_connect(void)
{
	int clt_skt;
	struct sockaddr_storage clt_addr;
    socklen_t addr_size = sizeof (clt_addr);
	
	clt_skt = accept(_srv_skt, (struct sockaddr *)(&clt_addr), &addr_size);
	if (clt_skt == -1)
		throw (std::runtime_error("accept: " + std::string(strerror(errno))));	
	if (fcntl(clt_skt, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("fcntl: " + std::string(strerror(errno))));		
	add_pfds(_pfds, clt_skt, POLLIN | POLLHUP);
	try
	{
		add_clients(_clients, clt_skt, std::string(printable_ip(clt_addr, clt_skt)));
	}
	catch (const std::exception& e) { throw; }	
}

/********* check inet_ntop authorised ***********/
std::string Server::printable_ip(struct sockaddr_storage client_addr, int clt_skt)
{
	char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string	
	char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
	
	if (client_addr.ss_family == AF_INET) // IPv4
	{
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)((struct sockaddr *)(&client_addr));
		if (!inet_ntop(AF_INET, &((*ipv4).sin_addr), ip4, INET_ADDRSTRLEN))			
			throw (std::runtime_error("inet_ntop (ipv4): " + std::string(strerror(errno))));	
		std::cout << "New connection from " << ip4 << " on socket " << clt_skt << std::endl;
		return(std::string(ip4));
	}
	else
	{
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)((struct sockaddr *)(&client_addr));
		if (!inet_ntop(AF_INET6, &((*ipv6).sin6_addr), ip6, INET6_ADDRSTRLEN))
			throw (std::runtime_error("inet_ntop (ipv6): " + std::string(strerror(errno))));	
		std::cout << "New connection from " << ip6 << " on socket " << clt_skt << std::endl;
		return(std::string(ip6));
	}	
}

void Server::add_clients(std::vector<Client>& clients, int clt_skt, std::string ip)
{
	Client new_clt;
	
	new_clt.set_clt_skt(clt_skt);
	new_clt.set_hostname(ip);
	clients.push_back(new_clt);	
}

void Server::add_pfds(std::vector<struct pollfd>& pfds, int fd, short events)
{
	struct pollfd new_skt;
	
	new_skt.fd = fd;
	new_skt.events = events;  // POLLIN = Tell me when ready to read
	new_skt.revents = 0;
	pfds.push_back(new_skt);	
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

bool Server::_signal = false; // Definition and initialization here for a static

void Server::handle_signal(int signal)
{
    // (void)signal;
	std::cout << "\nSignal " << signal << " received" << std::endl;
    _signal = true;
}

/**** draft ****/

// throw(Bureaucrat::GradeTooHighException()); /*************** */
// exception
// const char* Server::InitException::what() const throw()
// {
//     return (" ................ ");    
// }