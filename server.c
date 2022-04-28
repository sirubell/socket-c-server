#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "game.h"

#define PORT "15470"  // the port users will be connecting to
#define BACKLOG 10	 // how many pending connections queue will hold

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr *sa);
void* _start_server(void* arg);
void* handle_client(void* arg);

void start_server(void)
{
    pthread_t t;
    pthread_create(&t, NULL, _start_server, NULL);
}

void* _start_server(void* arg)
{
    (void)arg;

    int serverfd;
	struct addrinfo hints, *servinfo, *p;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
    // no need this, because I specify the node
	// hints.ai_flags = AI_PASSIVE; // use my IP

    int rv;
	if ((rv = getaddrinfo("0.0.0.0", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

        int yes = 1;
		if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("server: setsockopt");
			exit(1);
		}

		if (bind(serverfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(serverfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(serverfd, BACKLOG) == -1) {
		perror("server: listen");
		exit(1);
	}

    inet_ntop(p->ai_family, get_in_addr(p->ai_addr), s, sizeof(s));
    printf("server: bind to %s\n", s);
	printf("server: waiting for connections...\n");

	while (true) {
        struct sockaddr_storage client_addr;
        socklen_t sin_size = sizeof(client_addr);

		int client_fd = accept(serverfd, (struct sockaddr *)&client_addr, &sin_size);
		if (client_fd == -1) {
			perror("server: accept");
			continue;
		}

		inet_ntop(client_socket_addr.ss_family,
			get_in_addr((struct sockaddr *)&client_addr),
			s, sizeof(s));
		printf("server: got connection from %s\n", s);

        pthread_t client_thread;
		int* tmp = malloc(sizeof(int));
		*tmp = client_fd;
        pthread_create(&client_thread, NULL, handel_client, tmp);
	}
}

void* handle_client(void* arg)
{
	int clientfd = *(int*)arg;
	free(arg);

	
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}