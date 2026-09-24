#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "networking.h"
int main(void) {
	struct req_message request;
	struct request_line req_line;
	char method[] = "TEST";
	strcpy(req_line.method, method);
	(request.start_line) = &req_line;
	print_req_message(request);

	return 0;
}
