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

	int sl_size = get_str_size(sl.version) + get_str_size(sl.status) + get_str_size(sl.reason_phrase);
	sendbuf_ptr += sl_size;

	if (stat->field_str != NULL) {
		sprintf(sendbuf_ptr, "%s\n", stat->field_str);
		sendbuf_ptr += get_str_size(stat->field_str);
	}

	if (stat->body != NULL) {
		sprintf(sendbuf_ptr, "\n%s\n", stat->body);
		sendbuf_ptr += get_str_size(stat->body);
	}

	//printf("Sending:-%s-", sendbuf);
	send(sockfd, sendbuf, get_str_size(sendbuf) * sizeof(char), 0); // only send required bytes
	return 0;
}

void free_request(struct request *req) {
	free(req->req_line.method);
	free(req->req_line.target);
	free(req->req_line.version);
}

void free_status(struct status *stat) {
	free(stat->stat_line.version);
	free(stat->stat_line.status);
	free(stat->stat_line.reason_phrase);
	free(stat->field_str);
}

// receive data into buffer, parse it with parse_request()
void handle_connection(int sockfd) {
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	recv(sockfd, buf, sizeof(buf), 0);

	struct request req;
	//init_request(&req);
	struct request_line rl;
	memset(&rl, 0, sizeof(rl));
	parse_request(buf, &(rl)); 
	req.req_line = rl;

	// roll into stat_init function?
	struct status stat;
	memset(&stat, 0, sizeof(stat));
	//init_status(&stat);
	select_method(&req, &stat); // passes request to http module

	send_status(sockfd, &stat);
	// put these info funcs
	free_request(&req);
	free_status(&stat);
	/*
	free(req.body);
	free(stat.body);
	*/
}
