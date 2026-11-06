#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "http.h"
#include "networking.h"
#include "structs.h"

#define RSC_PATH "/home/user/programming/http-server/rsc"

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
		set_str(&(sl.status), "404");
		set_str(&(sl.reason_phrase), "Page Not Found");
		stat->stat_line = sl;
		return 0;
	}

	int buf_size = 2048;
	int offset = 0;

	void * tmp;
	//void * tmp = realloc(stat, buf_size * 2);
	//stat->body = malloc(buf_size);

	// fill start line
	set_str(&(sl.status), "200");
	set_str(&(sl.reason_phrase), "Success");
	stat->stat_line = sl;

	// fill header fields
	set_str(&(stat->field_str), "Content-Type: text/html");
	
	// fill body section
	stat->body = malloc(buf_size);
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
	set_str(&(stat->stat_line.version), "HTTP/1.1");

	struct request_line rl = req->req_line;
	if (!strcmp(rl.method, "GET")) {
		get(req, stat);
	} else {
		set_str(&(stat->stat_line.status), "501");
		set_str(&(stat->stat_line.reason_phrase), "Not Implemented");
	}
}
