#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main(int argc, char **argv)
{
	struct addrinfo *hints;
	struct addrinfo *res;
	int status;

	status = getaddrinfo(const char *node, const char *service, hints, &res);



}

struct addrinfo {
	int              ai_flags;
	int              ai_family;
	int              ai_socktype;
	int              ai_protocol;
	socklen_t        ai_addrlen;
	struct sockaddr *ai_addr;
	char            *ai_canonname;
	struct addrinfo *ai_next;
};
