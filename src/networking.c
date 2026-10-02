#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "networking.h"
#include "http.h"

#define REQUEST_SIZE 68

// creates listener socket, binds and listens
int listener_init(char *ip, int port, int backlog) {
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(addr.sin_addr));

	int yes = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
			perror("setsockopt");
			exit(1);
	}

	bind(listen_fd, (struct sockaddr *) &addr, sizeof(addr));
	listen(listen_fd, backlog);

	return listen_fd;
}

int send_status(int sockfd, struct status *stat) {
	// intermittent segfault on 404?
	char sendbuf[1024];
	char *sendbuf_ptr = sendbuf;
	memset(sendbuf, 0, sizeof(sendbuf));

	struct status_line sl = stat->stat_line;
	sprintf(sendbuf_ptr, "%s %s %s\n", sl.version, sl.status, sl.reason_phrase);
	sendbuf_ptr += sizeof(sl);

	if (strlen(stat->field_str) != 0) {
		sprintf(sendbuf_ptr, "%s\n", stat->field_str);
		printf("%s\n", sendbuf);
	}
	sendbuf_ptr += strlen(stat->field_str);

	if (stat->body != NULL) {
		sprintf(sendbuf_ptr, "%s\n", stat->body);
		sendbuf_ptr += sizeof(stat->body);
	}

	printf("Sending:\n%.1024s\n", sendbuf);
	send(sockfd, sendbuf, sizeof(sendbuf), 0);
}

// receive data into buffer, parse it with parse_request()
void handle_connection(int sockfd) {
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	recv(sockfd, buf, sizeof(buf), 0);

	struct request *req = (struct request *) malloc(REQUEST_SIZE); // why not sizeof(struct request *)?
								       // why malloc?
	memset(req, 0, sizeof(req));
	parse_request(buf, req);

	// roll into stat_init function?
	//struct status *stat = (struct status *) malloc(1024); 
	struct status stat;
	strcpy(stat.field_str, "");
	select_method(req, &stat); // passes request to http module

	send_status(sockfd, &stat);

	free(req);
}
