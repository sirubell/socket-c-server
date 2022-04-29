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
#include <assert.h>

#include "server.h"
#include "game.h"

#define PORT "15070"  // the port users will be connecting to
#define BACKLOG 10	 // how many pending connections queue will hold

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr *sa);
void* _start_server(void* arg);
void* handel_client(void* arg);
bool send_str(int socket, Str str);

void start_server(void)
{
    pthread_t t;
    pthread_create(&t, NULL, _start_server, NULL);
}

void* _start_server(void* arg)
{
    (void)arg;

    int server_socket;
	struct addrinfo hints, *servinfo, *p;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
    // no need this, because I specify the node
	hints.ai_flags = AI_PASSIVE; // use my IP

    int rv;
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(1);
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

        int yes = 1;
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("server: setsockopt");
			exit(1);
		}

		if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_socket);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(server_socket, BACKLOG) == -1) {
		perror("server: listen");
		exit(1);
	}

    inet_ntop(p->ai_family, get_in_addr(p->ai_addr), s, sizeof(s));
    printf("server: server socket fd: %d\n", server_socket);
    printf("server: bind to %s\n", s);
	printf("server: waiting for connections...\n");

	freeaddrinfo(servinfo); // all done with this structure

	while (true) {
        struct sockaddr_storage client_addr;
        socklen_t sin_size = sizeof(client_addr);

		int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);
		if (client_socket == -1) {
			perror("server: accept");
			continue;
		}

		inet_ntop(client_addr.ss_family,
			get_in_addr((struct sockaddr *)&client_addr),
			s, sizeof(s));
		printf("server: got connection from %s\n", s);

        pthread_t client_thread;
		int* tmp = malloc(sizeof(int));
		*tmp = client_socket;
        pthread_create(&client_thread, NULL, handel_client, tmp);
	}
}

#define BUFFER_CAPACITY 1024

void* handel_client(void* arg)
{
	int clientfd = *(int*)arg;
	free(arg);

	NodePlayer* node_player = NULL;
	Action create_player = {
		.type = CreatePlayer,
		.optint = clientfd,
	};
	action_push(create_player);

	char buf[BUFFER_CAPACITY];
	Str str;
	int nbytes;

	while (true) {
		if ((nbytes = recv(clientfd, buf, BUFFER_CAPACITY, 0)) < 0) {
			break;
		}

		if (node_player == NULL) {
			node_player = query_has(clientfd);
			if (node_player != NULL) {
				printf("server: player: %.*s joined the game\n", (int)node_player->p.name.len, node_player->p.name.s);
			}
		}

		Action change_dir = {
			.type = ChangePlayerDir,
			.optint = buf[0],
			.optptr = node_player,
		};
		action_push(change_dir);

		Str environment = get_environment();

		str_init(&str);
		if (node_player != NULL) {
			str_cat(&str, &node_player->p.name);
		}
		str_cat_cstr(&str, "\n");
		str_cat(&str, &environment);

		printf("server: send: %.*s\n", (int)str.len, str.s);
		if (!send_str(clientfd, str)) {
			break;
		}
		
	}

	if (node_player != NULL)
		printf("server: player: %.*s disconnected\n", (int)node_player->p.name.len, node_player->p.name.s);

	Action delete_player = {
		.type = DeletePlayer,
		.optptr = node_player,
	};
	action_push(delete_player);


	pthread_exit(NULL);
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool send_str(int socket, Str str)
{
	int total = 0, nbytes;
	while (total < str.len) {
		if ((nbytes = send(socket, str.s + total, str.len - total, 0)) < 0) {
			return false;
		}
		total += nbytes;
	}
	return true;
}