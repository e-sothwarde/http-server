#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include "http.h"
#include "networking.h"

#define RSC_PATH "/home/user/Programming/http-server/rsc"

int get(struct request *req, struct status *stat) {
	char target[TARGET_SIZE + sizeof(RSC_PATH)];
	sprintf(target, "%s%s", RSC_PATH, req->req_line.target);
	printf("GET %s\n", target);
	int fd = open(target, 0);
	if (fd == -1) {
		strcpy(stat->stat_line.status, "404");
		strcpy(stat->stat_line.reason_phrase, "Page Not Found");
		return 0;
	}

	// read file and pack into status?!

	strcpy(stat->stat_line.status, "200");
	strcpy(stat->stat_line.reason_phrase, "Success");
}


int handle_request(struct request *req, struct status *stat) {
	struct request_line rl = req->req_line;
	strcpy(stat->stat_line.version, "HTTP/1.1");

	if (!strcmp(rl.method, "GET")) {
		get(req, stat);
	} else {
		strcpy(stat->stat_line.status, "501");
		strcpy(stat->stat_line.reason_phrase, "Not Implemented");
	}
}
