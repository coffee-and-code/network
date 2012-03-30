#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
	struct addrinfo hints, *res;
	int status, sockfd;

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

	if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		close(sockfd);
		perror("connect");
		exit(1);
	}

	char *message = (char *)calloc(0, 256);

	while (1) {
		printf("message: ");
		fgets(message, 256, stdin);
		
		if (send(sockfd, message, strlen(message), 0) == -1) {
			close(sockfd);
			perror("send");
			exit(1);
		}
	}

	close(sockfd);
	freeaddrinfo(res);

	return 0;
}
