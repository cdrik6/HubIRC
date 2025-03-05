/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 02:49:45 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/05 03:42:56 by caguillo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

struct addrinfo {
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC to use whatever
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // use 0 for "any"
    size_t           ai_addrlen;   // size of ai_addr in bytes
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    char            *ai_canonname; // full canonical hostname

    struct addrinfo *ai_next;      // linked list, next node
};

// #include <stdlib.h>
// #include <unistd.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
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
	for (p = res; p; p = (*res).ai_next) ///*************valgrind check fd here  */
	{
		// socket
		server_socket = socket((*res).ai_family, (*res).ai_socktype, (*res).ai_protocol);
		if (server_socket == -1)			
			return (perror("server_socket"), freeaddrinfo(res), KO); ///******* valgrind test here ***** */
			/**************no return  here */
			
		// "already in use"	error
		int yes = 1;
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
			/**************no exit  here */
		}
		
		// bind
		status = bind(server_socket, (*res).ai_addr, (*res).ai_addrlen); 	
		if (status == -1)
			perror("bind");			
		
		break; // we take the first good
	}
	
	// listen
	listen(server_socket, BACKLOG);

	
	freeaddrinfo(res);
}
