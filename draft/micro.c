#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
// #include <netinet/in.h>

#define PORT "4243"
#define OK 0
#define KO 1

int	main(void)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int status;
	int socket_server;
	int socket_client;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, PORT, &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (freeaddrinfo(res), KO);
	}

	// Parcours liste chaine ???
	socket_server = socket((*res).ai_family, (*res).ai_socktype);


}