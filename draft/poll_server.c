/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poll_server.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:20:46 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/11 21:12:33 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// creer la server_socket + socktopt + bind + free
// listen
// pollfd

// struct	addrinfo
// {
// 	int ai_flags;             // AI_PASSIVE, AI_CANONNAME, etc.
// 	int ai_family;            // AF_INET, AF_INET6, AF_UNSPEC to use whatever
// 	int ai_socktype;          // SOCK_STREAM, SOCK_DGRAM
// 	int ai_protocol;          // use 0 for "any"
// 	size_t ai_addrlen;        // size of ai_addr in bytes
// 	struct sockaddr *ai_addr; // struct sockaddr_in or _in6
// 	char *ai_canonname;       // full canonical hostname

// 	struct addrinfo *ai_next; // linked list, next node
// };
// struct pollfd {
//     int fd;         // the socket descriptor
//     short events;   // bitmap of events we're interested in
//     short revents;  // when poll() returns, bitmap of events that occurred
// };

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
// #include <netinet/in.h>

#define PORT "6667"
#define BACKLOG 10
#define OK 0
#define KO 1
#define PFD_SIZE 5
#define BUFFER_SIZE 256

int add_to_pfds(struct pollfd *pfds, int *fd_count, int *fd_size);
void printable_ip(struct sockaddr_storage client_addr, int clt_skt);
int del_i_pfds(struct pollfd *pfds, int i, int *fd_count);

int	get_srv_skt(void)
{
	int				srv_skt;
	struct addrinfo	hints;
	struct addrinfo	*res;
	struct addrinfo	*p;
	int				yes;

	yes = 1;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// addrinfo
	if (getaddrinfo(NULL, PORT, &hints, &res) != 0)
		return (KO);
	// socket + bind
	for (p = res; p; p = (*res).ai_next)
	{
		srv_skt = socket((*p).ai_family, (*p).ai_socktype, (*p).ai_protocol);
		if (srv_skt < 0)
			continue ;
		setsockopt(srv_skt, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(srv_skt, (*p).ai_addr, (*p).ai_addrlen) < 0)
			continue ;
		break ;
	}
	freeaddrinfo(res);
	if (p == NULL)
		return (-1);
	// listen
	if (listen(srv_skt, 10) == -1)
		return (-1);
	return (srv_skt);
}

int	main(void)
{
	// server
	int srv_skt;		
	srv_skt = get_srv_skt();	
	if (srv_skt == -1)
		return (KO);
	// tab of pollfd
	int fd_count = 0;
	int fd_size = PFD_SIZE;
	struct pollfd *pfds;
	// int i = -1;	
	pfds = malloc(fd_size * sizeof(struct pollfd)); // --> calloc
	if (!pfds)
		return(KO);		
	// while (i++ < fd_size)		
	// 	pfds[i] = (struct pollfd){0};		
	pfds[0].fd = srv_skt;
	pfds[0].events = POLLIN; // Tell me when ready to read
	fd_count++;
	// printf ("ici\n");
	// client ckecking
	int clt_skt;
	struct sockaddr_storage clt_addr;
    socklen_t addr_size;	
	int event_count = 0;
	char buff[BUFFER_SIZE];	
	while (1)
	{
		// poll returns the number of elements in the array that have had an event occur
		event_count = poll(pfds, fd_count, -1); // -1 = wait forever
		if (event_count == -1) 
			return (KO);
		// run through the existing connections looking for data to read
		for (int i = 0; i < fd_count; i++)
		{			
			if (pfds[i].revents & (POLLIN | POLLHUP)) // if got one ready to read
			{
				if (pfds[i].fd == srv_skt) // If srv_skt is ready to read, handle new connection
				{
					addr_size = sizeof (clt_addr);
					clt_skt = accept(srv_skt, (struct sockaddr *)(&clt_addr), &addr_size);
					if (clt_skt == -1)
						perror("accept");
					else
					{
						// add_to_pfds(pfds, &fd_count, &fd_size); // --> check error
						pfds[fd_count].fd = clt_skt;
						pfds[fd_count].events = POLLIN;
						fd_count++;						
						printable_ip(clt_addr, clt_skt);
					}
				}
				else // we act as a client (receive data)
				{
					int nbytes = recv(pfds[i].fd, buff, sizeof(buff), 0);
					if (nbytes <= 0) // issues
					{
						if (nbytes == 0)
							printf ("Socket %d closed the connection\n", pfds[i].fd);
						if (nbytes < 0)	
							perror ("recv");						
						close (pfds[i].fd);
						del_i_pfds(pfds, i, &fd_count);
					}
					else // got some data from a client --> to send the others (not srv not sender)
					{
						for (int j = 1; j < fd_count; j++)
						{
							// error below ? send when i == j ?
							if (j != i && send(pfds[j].fd, buff, nbytes, 0) == - 1)
								perror("send");							
						}						
					}
				}
			} // find one
		} // looking for data
	} // loop
	return (0);
}

void printable_ip(struct sockaddr_storage client_addr, int clt_skt)
{
	char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string	
	char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
	
	if (client_addr.ss_family == AF_INET) // IPv4
	{
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)((struct sockaddr *)(&client_addr));
		inet_ntop(AF_INET, &((*ipv4).sin_addr), ip4, INET_ADDRSTRLEN);			
		printf("New connection from %s on ", ip4);		
	}
	else
	{
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)((struct sockaddr *)(&client_addr));
		inet_ntop(AF_INET6, &((*ipv6).sin6_addr), ip6, INET6_ADDRSTRLEN);			
		printf("New connection from %s on ", ip6);		
	}
	printf ("socket: %d\n", clt_skt);		
}

// error !! 
int add_to_pfds(struct pollfd *pfds, int *fd_count, int *fd_size)
{	
	struct pollfd *temp;
	int temp_size = *fd_size;
	int i = -1;
	
	if (*fd_count == *fd_size)
	{
		temp = malloc((*fd_size) * sizeof(struct pollfd));
		if (!temp)
			return (KO);			
		while (i++ < *fd_size)		
			temp[i] = pfds[i];
		free (pfds);
		fd_size = fd_size + PFD_SIZE;
		pfds = malloc((*fd_size) * sizeof(struct pollfd)); // --> calloc
		i = -1;
		while (i++ < *fd_size)		
			pfds[i] = (struct pollfd){0};	
		i = -1;	
		while (i++ < temp_size)		
			pfds[i] = temp[i];
		free (temp);		
	}
	return (OK);
}

int del_i_pfds(struct pollfd *pfds, int i, int *fd_count)
{
	pfds[i] = pfds[(*fd_count) - 1];
	pfds[(*fd_count) - 1] = (struct pollfd){0};	
	(*fd_count)--;
	return (OK);
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