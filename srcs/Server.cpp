/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 00:32:58 by caguillo          #+#    #+#             */
/*   Updated: 2025/04/20 03:47:42 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Relais pour la causette Internet 
#include "Server.hpp"

Server::Server(char *port, std::string password)
{
	add_pfds(STDIN_FILENO, POLLIN); // add std_in en 0	
	_password = password;
	_srv_skt = create_srv_skt(port); 
	std::cout << "Server constructed on socket " << _srv_skt << std::endl;	
	add_pfds(_srv_skt, POLLIN); // server en 1	
	std::cout << "Server: waiting for connections...\n";
	std::cout << "Server: \"stop\" to stop it\n\n";
}

Server::Server()
{	
}

Server::~Server()
{
	/***** draft****** */
	std::cout << "Server destructor called\n";
	// close (_srv_skt); // i = 1
	for (int i = 1; i < _pfds.size(); i++)
	{
		close (_pfds.at(i).fd);
	}
	/************** */
}

// Server::Server(const Server& other)
// {
//     *this = other;
// }

// Server& Server::operator=(const Server& other)
// {
//     if (this == &other)
// 		return (*this);
//     _srv_skt = other._srv_skt;    
//     _password = other._password;	
//     _clts = other._clts;
//     _pfds = other._pfds;
//     _chnls = other._chnls;    
// 	return (*this);
// }

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
	for (p = res; p; p = (*p).ai_next)
	{		
		srv_skt = socket((*p).ai_family, (*p).ai_socktype, (*p).ai_protocol);
		if (srv_skt < 0)
			continue;
		if (setsockopt(srv_skt, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) // SO_KEEPALIVE --> to check dead/silent client
			(perror("setsockopt"), freeaddrinfo(res), exit (KO));				
		if (fcntl(srv_skt, F_SETFL, O_NONBLOCK) == -1)
			(perror("fcntl"), freeaddrinfo(res), exit (KO));
		if (bind(srv_skt, (*p).ai_addr, (*p).ai_addrlen) < 0)
			continue; // (perror("bind"), freeaddrinfo(res), exit (KO));
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
			char buff[BUFFER_SIZE + 1] = {0};
			int nbytes = read(STDIN_FILENO, buff, BUFFER_SIZE);
			if (nbytes == -1)
				throw (std::runtime_error("read: " + std::string(strerror(errno))));
			if (nbytes == 0 || std::string(buff, nbytes) == "stop\n")
				break;
			// check sockets in use
			if (std::string(buff, nbytes) == "socket\n")			
				for (int i = 0; i < _pfds.size(); i++)				
					std::cout << "Socket " << _pfds.at(i).fd << " is connected\n";
		}		
		
		// if server get sthg to read (--> new connection)	
		if (_pfds.at(1).revents & POLLIN)
			client_connect();
			
		// ckeck revents of clients 
		for (int i = 2; i < _pfds.size(); i++)
		{
			if (_pfds.at(i).revents & (POLLIN | POLLHUP)) // if got one ready to read
			{
				char buff[BUFFER_SIZE + 1] = {0};
				int nbytes = recv(_pfds.at(i).fd, buff, BUFFER_SIZE, 0);
				int k = client_idx(_pfds.at(i).fd);
				// Server output
				std::cout << "Received(" << _pfds.at(i).fd << ") [" << buff << "]\n" << std::endl;
				// std::cout << "(nbytes = " << nbytes << ")\n" << std::endl;
				if (nbytes <= 0) // closed or issues
				{
					if (nbytes == 0)						
						client_disconnect(":Connection closed by client", i, k);
					if (nbytes == -1)
					{
						// std::cout << "fd = " << _pfds.at(i).fd << std::endl;
						std::vector<int>::iterator it = std::find(_fails.begin(), _fails.end(), _pfds.at(i).fd);
            			if (it == _fails.end())
                			_fails.push_back(_pfds.at(i).fd);
						// throw (std::runtime_error("recv: " + std::string(strerror(errno))));
					}
						
				}
				else // got some data from a client
				{				
					if (parse_message(std::string(buff), k) == OK)
						if (check_registered(k) == OK)
							if (_clts.at(k).get_registered() == false)
								welcome(k);					 			
				}				
			}
		}
		// for (int j = 0; j < _fails.size(); j++)
		// 	std::cout << "fd = " << _fails.at(j) << std::endl;
		_fails.clear(); //clean_fails();
	}
}
// close fds in destructor here
// Note: flag in send ()
// if the socket has been closed by either side, the process calling send() will get the signal SIGPIPE.
// Unless send() was called with the MSG_NOSIGNAL flag.

int Server::check_registered(int clt_idx)
{
	if (_clts.at(clt_idx).get_registered() == true)
		return (OK);
	if (_clts.at(clt_idx).get_pwd_ok() == false)
		return (reply(COD_PASSWDMISMATCH, ERR_PASSWDREQUIRED, clt_idx), KO);
	if (_clts.at(clt_idx).get_nickname() == "*")
		return (reply(COD_NOTREGISTERED, ERR_NOTREGISTERED, clt_idx), KO);
	if (_clts.at(clt_idx).get_username() == "")
		return (reply(COD_NOTREGISTERED, ERR_NOTREGISTERED, clt_idx), KO);		
	return (OK);
}

// RFC 2812: message = [ ":" prefix SPACE ] command [ params ] CRLF
int Server::parse_message(std::string buffer, int clt_idx)
{	
	std::vector<std::string> tab_msg;	
	
	if (clt_idx != -1)	
		_clts.at(clt_idx).set_msg(buffer);	// build message	
	if (clt_idx != -1 && buffer.find("\r\n") != std::string::npos)
	{		
		tab_msg = split(_clts.at(clt_idx).get_msg());		
		if (tab_msg.size() == 0)
			return (std::cout << "tab empty" << std::endl, KO);
		for (int i = 0; i < tab_msg.size(); i++)
		{			
			// std::cout << i << " = [" << tab_msg[i] << "]" << std::endl;
			if (get_command(tab_msg, tab_msg[i], clt_idx, i) == KO)
				return (KO);
		}	
		_clts.at(clt_idx).clear_msg();		
		return (OK);
	}	
	return (KO);
}
// Note
// \n for nc or \r\n for irssi so find_first_of() not find() --> NO! nc -C sends \r\n
// RFC IRC: \n is invalid in command and parameters
// /msg #channel Hello, how are you? --> PRIVMSG #channel :Hello, how are you?\r\n
// /quote PRIVMSG #channel :Hello\nNew line? --> PRIVMSG #channel :Hello New line?\r\n

int Server::get_command(std::vector<std::string>& tab_msg, std::string& cmd, int clt_idx, int tab_idx)
{
	if (toUpper(cmd) == "PING")
		ping(tab_msg, clt_idx, tab_idx);
	else if (toUpper(cmd) == "NICK")
		nickname(tab_msg, clt_idx, tab_idx);
	else if (toUpper(cmd) == "PASS")
	 	pass(tab_msg, clt_idx, tab_idx);
	else if (toUpper(cmd) == "USER")
	 	username(tab_msg, clt_idx, tab_idx);
	else if (toUpper(cmd) == "QUIT")
	 	return (quit(tab_msg, clt_idx, tab_idx), KO);
	else if (_clts.at(clt_idx).get_registered() == true)
	{
		if (toUpper(cmd) == "PRIVMSG")		
			privmsg(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "JOIN")
			join(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "WHO")
			who(tab_msg, clt_idx, tab_idx);	
		else if (toUpper(cmd) == "PART")
			part(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "MODE")
			mode(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "TOPIC")
			topic(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "KICK")
			kick(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "INVITE")
			invite(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "NOTICE")
			notice(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "OPER")
			oper(tab_msg, clt_idx, tab_idx);
		else if (toUpper(cmd) == "IAMBOT")
			iambot(tab_msg, clt_idx, tab_idx);
		else if (tab_idx == 0)
			reply(COD_UNKNOWNCOMMAND, cmd + " " + ERR_UNKNOWNCOMMAND, clt_idx);
	}
	return (OK);
}

int Server::client_idx(int clt_skt)
{
	for (int i = 0; i < _clts.size(); i++)
	{		
		if (_clts.at(i).get_clt_skt() == clt_skt)			
			return (i);		
	}	
	return (-1); // disconnected in between or error ?
}

// std::cout << "Socket " << _clients.at(clt_idx).get_clt_skt() << " closed the connection\n";	
void Server::client_disconnect(std::string reason, int pfd_idx, int clt_idx)
{
	
	quit_channels(reason, clt_idx);
	std::cout << "Socket " << _pfds.at(pfd_idx).fd << " closed the connection\n";	
	close(_pfds.at(pfd_idx).fd);	
	_pfds.erase(_pfds.begin() + pfd_idx);	
	_clts.erase(_clts.begin() + clt_idx);	
}

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
	add_pfds(clt_skt, POLLIN | POLLHUP);	
	add_clients(clt_skt, std::string(printable_ip(clt_addr, clt_skt)));	
}

std::string Server::printable_ip(struct sockaddr_storage client_addr, int clt_skt)
{
	char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string	
	char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
	
	if (client_addr.ss_family == AF_INET) // IPv4
	{
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)((struct sockaddr *)(&client_addr));
		if (!inet_ntop(AF_INET, &((*ipv4).sin_addr), ip4, INET_ADDRSTRLEN))			
			throw (std::runtime_error("inet_ntop (ipv4): " + std::string(strerror(errno))));	
		std::cout << "New connection from " << ip4 << " on socket " << clt_skt << std::endl << std::endl;		
		return(std::string(ip4));
	}
	else
	{
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)((struct sockaddr *)(&client_addr));
		if (!inet_ntop(AF_INET6, &((*ipv6).sin6_addr), ip6, INET6_ADDRSTRLEN))
			throw (std::runtime_error("inet_ntop (ipv6): " + std::string(strerror(errno))));	
		std::cout << "New connection from " << ip6 << " on socket " << clt_skt << std::endl << std::endl;
		return(std::string(ip6));
	}	
}

void Server::clean_fails(void)
{
	int clt_idx = -1;
	int pfd_idx = -1;
	
	for (int i = 0; i < _fails.size(); i++)
	{		
		for (int j = 0; j < _chnls.size(); j++)
		{
			_chnls.at(j).rem_operator(_fails.at(i));
			_chnls.at(j).rem_invitee(_fails.at(i));
			_chnls.at(j).rem_chnlclt(_fails.at(i));
		}
		clt_idx = client_idx(_fails.at(i));
		if (clt_idx != -1)		
			_clts.erase(_clts.begin() + clt_idx);					
		for (int j = 2; j < _pfds.size(); j++)
    	{
        	if (_pfds.at(j).fd == _fails.at(i))
        	{
            	pfd_idx = j;
            	break;
        	}            
    	}    
    	if (pfd_idx != -1)
			_pfds.erase(_pfds.begin() + pfd_idx);		
	}	
	_fails.clear();	
}

void Server::add_clients(int clt_skt, std::string ip)
{
	Client new_clt;
		
	new_clt.set_clt_skt(clt_skt);
	new_clt.set_hostname(ip);
	_clts.push_back(new_clt);		
}

void Server::add_pfds(int fd, short events)
{
	struct pollfd new_skt;
	
	new_skt.fd = fd;
	new_skt.events = events;  // POLLIN = Tell me when ready to read
	new_skt.revents = 0;
	_pfds.push_back(new_skt);	
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
    std::cout << "\nSignal " << signal << " received" << std::endl;
    _signal = true;
}
/**
 * SIGINT = ctrl+c
 * SIGQUIT = ctrl+\
 * ctrl+d = not a signal
**/