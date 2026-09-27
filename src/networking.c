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

int parse_request(char *recv_buf, struct request *req) {
	sscanf(recv_buf, "%s %s %s\n", req->req_line.method, req->req_line.target, req->req_line.version);
}

int print_request(struct request *req) {
	printf("%s %s %s\n", req->req_line.method, req->req_line.target, req->req_line.version);
}

/*
int print_status(struct status *stat) {
	printf("%s %s %s\n", stat->version, stat->status, stat->reason_phrase);
}

*/
int send_status_line(int sockfd, struct status *stat) {
	char sendbuf[1024];
	memset(sendbuf, 0, sizeof(sendbuf));

	struct status_line sl = stat->stat_line;
	sprintf(sendbuf, "%s %s %s\n", sl.version, sl.status, sl.reason_phrase);
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

	struct status *stat = (struct status *) malloc(STATUS_SIZE); 
	handle_request(req, stat); // passes request to http module

	send_status_line(sockfd, stat);

	free(req);
}
