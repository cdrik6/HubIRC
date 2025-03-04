#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define closesocket(param) close(param)
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef enum Bool Bool;
enum Bool {
    FALSE, TRUE
};
struct Client {
    SOCKET *sock;
    int *nb_clients;
};

void* tClient(void* arg);

int main() {
	pthread_t cThread[5];
	SOCKET sSock;
	SOCKADDR_IN sSin;

	sSock = socket(AF_INET, SOCK_STREAM, 0);
	if(sSock != INVALID_SOCKET) {
		sSin.sin_addr.s_addr = htonl(INADDR_ANY);
		sSin.sin_family = AF_INET;
		sSin.sin_port = htons(42000);
		printf("bind()...\n");
		if(bind(sSock, (SOCKADDR*)&sSin, sizeof(sSin)) != SOCKET_ERROR) {
			printf("listen()...\n");
			if(listen(sSock, 5) != SOCKET_ERROR) {
				printf("Attente de clients...\n");
				SOCKET cSock[5];
                SOCKADDR_IN cSin;
                socklen_t sizeCSin = sizeof(cSin);

                int nb_clients = 0;
                Bool allume = TRUE;
                struct Client tTClient[5];
                while(allume = TRUE) {
                    tTClient[nb_clients].sock = &cSock[nb_clients];
                    tTClient[nb_clients].nb_clients = &nb_clients;
                    cSock[nb_clients] = accept(sSock, (SOCKADDR*)&cSin, &sizeCSin);
                    pthread_create(&cThread[nb_clients], NULL, tClient, &tTClient[nb_clients]);
                    nb_clients++;
                }
                pthread_join(cThread[0], NULL);
                pthread_join(cThread[1], NULL);
                pthread_join(cThread[2], NULL);
                pthread_join(cThread[3], NULL);
                pthread_join(cThread[4], NULL);
            } else {
				perror("Error listen()");
			}
		} else {
			perror("Error bind()");
		}
	} else {
		perror("Error socket()");
	}

	printf("Fermeture de la socket...\n");
	closesocket(sSock);

	return EXIT_SUCCESS;
}

void* tClient(void* arg) {
	printf("Un client s'est connecté...\n");
	char buffer[4096] = "Bienvenue sur le Salon 127001, est-ce votre premiere connexion ? Repondez oui ou non en minuscules";
	SOCKET *cSock;
	cSock = ((struct Client *)arg)->sock;
	if(send(*cSock, buffer, 4096, 0) != SOCKET_ERROR) {
		printf("<this*> : %s\n", buffer);
	} else {
		perror("Error send()");
	}
	if(recv(*cSock, buffer, 4096, 0) != SOCKET_ERROR) {
		printf("<new client> : %s\n", buffer);
	} else {
		perror("Error recv()");
	}
	shutdown(*cSock, 2);
	int *nb_clients;
    nb_clients = ((struct Client *)arg)->nb_clients;
	*nb_clients--; //J'espere que ca va fonctionner
	return NULL;
}
