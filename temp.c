#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#define PORT "15470"  // the port users will be connecting to
#define BACKLOG 10	 // how many pending connections queue will hold

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void open_server(void);
void* start_listening(void*);

int main(void)
{
    open_server();

	return 0;
}

void open_server(void)
{
	int server_socket_fd;
	struct addrinfo hints, *servinfo, *p;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
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
		if ((server_socket_fd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

        int yes = 1;
		if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(server_socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_socket_fd);
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

	if (listen(server_socket_fd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

    inet_ntop(p->ai_family, get_in_addr(p->ai_addr), s, sizeof s);
    printf("server: bind to %s\n", s);
	printf("server: waiting for connections...\n");

    pthread_t t;
    pthread_create(&t, NULL, start_listening, &server_socket_fd);
}

void* start_listening(void *arg) 
{
	char s[INET6_ADDRSTRLEN];
    int server_socket_fd = *(int*)arg;

	while(1) {
        struct sockaddr_storage client_socket_addr;
        socklen_t sin_size = sizeof(client_socket_addr);

		int client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_socket_addr, &sin_size);
		if (client_socket_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(client_socket_addr.ss_family,
			get_in_addr((struct sockaddr *)&client_socket_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handel_client, &client_socket_fd);
	}
}
