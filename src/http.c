#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "http.h"
#include "networking.h"

#define RSC_PATH "/home/user/programming/http-server/rsc"

int parse_request(char *recv_buf, struct request *req) {
	sscanf(recv_buf, "%s %s %s\n", req->req_line.method, req->req_line.target, req->req_line.version);
}

int print_request(struct request *req) {
	printf("%s %s %s\n", req->req_line.method, req->req_line.target, req->req_line.version);
}

// prints status line
int print_status(struct status *stat) {
	printf("%s %s %s\n", stat->stat_line.version, stat->stat_line.status, stat->stat_line.reason_phrase);
}

int get(struct request *req, struct status *stat) {
	char target_path[TARGET_SIZE + sizeof(RSC_PATH)];
	if (!strcmp(req->req_line.target, "/")) {
//		memset(req->req_line.target, 0, TARGET_SIZE);
		strcpy(req->req_line.target, "/index.html");
	}
	
	sprintf(target_path, "%s%s", RSC_PATH, req->req_line.target);
	//printf("GET %s\n", target);
	
	struct status_line sl = stat->stat_line;

	int fd = open(target_path, 0);
	if (fd == -1) {
		strcpy(sl.status, "404");
		strcpy(sl.reason_phrase, "Page Not Found");
		stat->stat_line = sl;
		return 0;
	}

	int buf_size = 2048;
	int offset = 0;

	void * tmp;
	//void * tmp = realloc(stat, buf_size * 2);
	//stat->body = malloc(buf_size);

	// fill start line
	strcpy(sl.status, "200");
	strcpy(sl.reason_phrase, "Success");
	stat->stat_line = sl;

	// fill header fields
	strcpy(stat->field_str, "Content-Type: text/html");
	
	// fill body section
	while (read(fd, (stat->body+offset), 8) != 0) {
		offset += 8;
		if (offset == buf_size) {
			// resizing is causing status line to get corrupted
			buf_size = buf_size * 2;
			tmp = realloc(stat, buf_size * 2);
			tmp = realloc(stat->body, buf_size);
		}
	}

}

int select_method(struct request *req, struct status *stat) {
	struct request_line rl = req->req_line;
	strcpy(stat->stat_line.version, "HTTP/1.1");

	if (!strcmp(rl.method, "GET")) {
		get(req, stat);
	} else {
		strcpy(stat->stat_line.status, "501");
		strcpy(stat->stat_line.reason_phrase, "Not Implemented");
	}
}
