#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAXDATASIZE 100

int main(int argc, char *argv[]) {
	struct addrinfo hints, *res;
	int status, sockfd, newfd, numbytes;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		return 1;
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo("localhost", argv[1], &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
		perror("socket");
		exit(1);
	}

	if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, 10) == -1) {
		perror("listen");
		exit(1);
	}

	addr_size = sizeof their_addr;

	if ((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size)) == -1) {
		perror("accept");
		exit(1);
	}
	
	char buf[MAXDATASIZE];

	if ((numbytes = recv(newfd, buf, MAXDATASIZE - 1, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';

	printf("message: \"%s\"", buf);

	close(sockfd);
	close(newfd);

	freeaddrinfo(res);

	return 0;
}
