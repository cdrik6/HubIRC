#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define closesocket(param) close(param)
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

int main() {
	int cSock = socket(AF_INET, SOCK_DGRAM, 0);
	if(cSock != INVALID_SOCKET) {
		struct sockaddr_in cAddr;
		cAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		cAddr.sin_family = AF_INET;
		cAddr.sin_port = htons(42000);

		printf("connect()...\n");
		if(connect(cSock, (SOCKADDR*)&cAddr, sizeof(cAddr)) != SOCKET_ERROR) {
			char buffer[4096] = "";
			if(recv(cSock, buffer, 4096, 0) != SOCKET_ERROR) {
				printf("<server> : %s\n", buffer);
			} else {
				perror("Error recv()");
			}
			printf("<you*> : ");
			scanf("%s", buffer);
			if(send(cSock, buffer, 4096, 0) != SOCKET_ERROR) {
				printf("\n");
			} else {
				perror("Error send()");
			}
		} else {
			perror("Error connect()");
		}
		closesocket(cSock);
	} else {
		perror("socket()");
	}

	return EXIT_SUCCESS;
}
