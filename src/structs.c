#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

int init_request(struct request *req) {
	req->req_line.method = malloc(0);
	req->req_line.target = malloc(0);
	req->req_line.version = malloc(0);

	req->body = malloc(0);
}

int init_status(struct status *stat) {
	stat->stat_line.version = malloc(0);
	stat->stat_line.status = malloc(0);
	stat->stat_line.reason_phrase = malloc(0);

	stat->field_str = malloc(0);
	stat->body = malloc(0);
}

 // returns length of string in bytes
int get_str_size(char *str) {
	int i = 0;
	while (str[i] != '\0') {
		i++;}
	return i+1;
}

 // resizes dest and copies src into it
 // add error checking here
int set_str(char **dest, char *src) {
	int str_size = get_str_size(src);
	char *tmp;
	tmp = malloc(str_size);
	/*
	if (*dest == NULL) {
		tmp = malloc(str_size);
	} else {
		tmp = realloc(*dest, str_size);
	}
	*/
	*dest = tmp;
	strcpy(*dest, src);
}

int parse_request(char *recv_buf, struct request_line *rl) {
	char method[256], target[256], version[256];
	sscanf(recv_buf, "%s %s %s\n", method, target, version);
	set_str(&(rl->method), method);
	set_str(&(rl->target), target);
	set_str(&(rl->version), version);
}

int print_request(struct request *req) {
	printf("%s %s %s\n", req->req_line.method, req->req_line.target, req->req_line.version);
}

// prints status line
int print_status(struct status *stat) {
	printf("%s %s %s\n", stat->stat_line.version, stat->stat_line.status, stat->stat_line.reason_phrase);
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
