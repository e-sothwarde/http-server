#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "networking.h"
#include "http.h"
#include "structs.h"

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

int main(void) {
	int listener = listener_init("127.0.0.1", 3005, 10);

	struct sockaddr conn_addr;
	socklen_t conn_addr_size = sizeof(conn_addr);
	int connected_fd;
	for (int i = 0; i < 10; i++) {
		connected_fd = accept(listener, &conn_addr, &conn_addr_size);

		handle_connection(connected_fd);
		close(connected_fd);
	}

	close(listener);
	return 0;
}
