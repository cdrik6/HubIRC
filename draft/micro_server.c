/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_server.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 02:49:45 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/06 03:58:27 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#include <stdlib.h>
#include <unistd.h>
// #include <netinet/in.h>
#include <arpa/inet.h>
// #include <sys/wait.h>
// #include <signal.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT "1025"
#define BACKLOG 10
#define OK 0
#define KO 1

/* lost of forbidden functions !!! */
int	main(void)
{
	struct addrinfo	hints;
	struct addrinfo	*res;
	struct addrinfo	*p;
	int				status;
	int				server_socket;
	int				client_socket;	

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, PORT, &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (freeaddrinfo(res), KO); ///******* valgrind test here ***** */
	}
	// Linked list of res, we take the first good
	for (p = res; p; p = (*res).ai_next) ///****valgrind check fd here  */
	{
		// socket
		server_socket = socket((*p).ai_family, (*p).ai_socktype, (*p).ai_protocol);
		if (server_socket == -1)
		{
			perror("server_socket");
			continue ;
		}
		// // "already in use"	error
		// int yes;
		// yes = 1;
		// if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes,
		// 		sizeof(int)) == -1)
		// {
		// 	perror("setsockopt");
		// 	exit(1); /**************no exit  here ??? */
		// }
		// bind		
		if (bind(server_socket, (*p).ai_addr, (*p).ai_addrlen) == -1)
		{
			perror("bind");
			continue;
		}			
		break; // we take the first good
	}
	freeaddrinfo(res);
	// listen
	if (listen(server_socket, BACKLOG) == -1)	
		(perror("listen"), exit (KO));
	//	
	printf("server: waiting for connections...\n");
	//
	struct sockaddr_storage client_addr;
    socklen_t addr_size;
	char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string	
	char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
	while (1)
	{
		client_socket = accept(server_socket, (struct sockaddr *)(&client_addr), &addr_size);
		if (client_socket == -1)
		{
			perror("accept");
			continue;
		}
		if (client_addr.ss_family == AF_INET) // IPv4
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)((struct sockaddr *)(&client_addr));
			inet_ntop(AF_INET, &((*ipv4).sin_addr), ip4, INET_ADDRSTRLEN);			
			printf("Connection from %s\n", ip4);
		}
		else
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)((struct sockaddr *)(&client_addr));
			inet_ntop(AF_INET6, &((*ipv6).sin6_addr), ip6, INET6_ADDRSTRLEN);			
			printf("Connection from %s\n", ip6);
		}		
		printf("server_socket: %d, client_socket: %d\n", server_socket, client_socket);			
		//
		if (send(client_socket, "Hello world!", 12, 0) == -1)
                perror("send");            
		close(client_socket);
	}	
}
