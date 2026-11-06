#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "networking.h"
#include "http.h"
#include "structs.h"

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

	sendbuf_ptr += get_str_size(sl.version);
	sendbuf_ptr += get_str_size(sl.status);
	sendbuf_ptr += get_str_size(sl.reason_phrase);

	if (stat->field_str != NULL) {
		sprintf(sendbuf_ptr, "%s\n", stat->field_str);
		sendbuf_ptr += get_str_size(stat->field_str);
	}

	if (stat->body != NULL) {
		sprintf(sendbuf_ptr, "\n%s\n", stat->body);
		sendbuf_ptr += get_str_size(stat->body);
	}

	send(sockfd, sendbuf, get_str_size(sendbuf) * sizeof(char), 0);	return 0;
}
