/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_client.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguillo <caguillo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 03:00:51 by caguillo          #+#    #+#             */
/*   Updated: 2025/03/06 03:53:39 by caguillo         ###   ########.fr       */
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
#define MAXBYTES 100
#define OK 0
#define KO 1

int	main(int argc, char **argv)
{
	(void)argc;
    struct addrinfo	hints;
	struct addrinfo	*res;
	struct addrinfo	*p;
	int				status;	
	int				client_socket;	

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;	
	status = getaddrinfo(argv[1], PORT, &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (freeaddrinfo(res), KO); ///******* valgrind test here ***** */
	}    
	// Linked list of res, we take the first good
	for (p = res; p; p = (*res).ai_next) ///****valgrind check fd here  */
	{
		// socket
		client_socket = socket((*p).ai_family, (*p).ai_socktype, (*p).ai_protocol);
		if (client_socket == -1)
		{
			perror("client_socket");
			continue ;
		}
        // connect
        if (connect(client_socket, (*p).ai_addr, (*p).ai_addrlen) == -1)
        {
            close(client_socket);
            perror("client: connect");
            continue;
        }        
		break; // we take the first good
	}    
    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return (KO);
    }    
    //
    char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string	
	char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string   
    if ((*p).ai_family == AF_INET) // IPv4
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)((*p).ai_addr);
        inet_ntop(AF_INET, &((*ipv4).sin_addr), ip4, INET_ADDRSTRLEN);			
        printf("Connection from %s\n", ip4);
    }
    else
    {
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)((*p).ai_addr);
        inet_ntop(AF_INET6, &((*ipv6).sin6_addr), ip6, INET6_ADDRSTRLEN);			
        printf("Connection from %s\n", ip6);
    }    
	freeaddrinfo(res);    
    //
    int numbytes;
    char buff[MAXBYTES];
    if ((numbytes = recv(client_socket, buff, MAXBYTES-1, 0)) == -1)
    {
        perror("recv");
        exit(KO);
    }
    buff[numbytes] = '\0';
    printf("client: received '%s'\n",buff);
    close(client_socket);
}