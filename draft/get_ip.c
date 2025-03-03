#include "structures.h"

int main(int argc, char **argv)
{
	// struct sockaddr_in sa; // IPv4
	// struct sockaddr_in6 sa6; // IPv6

	// // functions that allow you to manipulate IP addresses
	
	// // printable to network
	// // int inet_pton(int af, const char *src, void *dst);	
	// inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr)); // IPv4
	// inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); // IPv6

	// // network to printable
	// const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
	// char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string
	// char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
	// // struct sockaddr_in sa;      // pretend this is loaded with something
	// // struct sockaddr_in6 sa6;    // pretend this is loaded with something
	// inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
	// inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);
	// printf("The IPv4 address is: %s\n", ip4);
	// printf("The IPv6 address is: %s\n", ip6);

	struct addrinfo hints;
	struct addrinfo *res;
	int status;
	// const char *node;
	// const char *service;
	// node = "www.example.com" or IP
	// service = "http" or port number

	// status = getaddrinfo(node, service, &hints, &res);
	
	memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	// hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(KO);
	}
	// printf("ai_flags = %d\n", (*res).ai_flags);
	// printf("ai_family = %d\n", (*res).ai_family);
	// printf("ai_socktype = %d\n", (*res).ai_socktype);
	// printf("ai_protocol = %d\n", (*res).ai_protocol);
	// printf("ai_addrlen = %u\n", (*res).ai_addrlen);  
    // printf("ai_canonname = %s\n", (*res).ai_canonname);  
    // // struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    // printf("ai_addr sa_family = %d\n", (*((*res).ai_addr)).sa_family);  
	// if ((*((*res).ai_addr)).sa_data[0])
	// 	printf("ai_addr sa_data = %s\n", (*((*res).ai_addr)).sa_data);  
    
	char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string	
	char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
	// printf("%s\n", inet_ntop((*res).ai_family, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN));
	// const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

	struct addrinfo *p;
	p = res;
	// for (p = res; p != NULL; p = (*p).ai_next)
	while (p)
	{
		if ((*p).ai_family == AF_INET) // IPv4
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)((*p).ai_addr);
			// printf("%s\n",inet_ntop(AF_INET, &((*ipv4).sin_addr), ip4, INET_ADDRSTRLEN));
			inet_ntop(AF_INET, &((*ipv4).sin_addr), ip4, INET_ADDRSTRLEN);
			printf("The IPv4 address is: %s\n", ip4);
		}
		else // IPv6
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)((*p).ai_addr);
			// printf("%s\n",inet_ntop(AF_INET6, &((*ipv6).sin6_addr), ip6, INET6_ADDRSTRLEN));
			inet_ntop(AF_INET6, &((*ipv6).sin6_addr), ip6, INET6_ADDRSTRLEN);
			printf("The IPv6 address is: %s\n", ip6);
		}
		p = (*p).ai_next;
	}


	freeaddrinfo(res); // free the linked-list

	return (0);
}