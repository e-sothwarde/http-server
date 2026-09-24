#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "networking.h"
#include "http.h"

int main(void) {
	int listener = listener_init("192.168.64.2", 3005, 10);

	struct sockaddr conn_addr;
	socklen_t conn_addr_size = sizeof(conn_addr);
	int connected_fd;
	for (int i = 0; i < 10; i++) {
		connected_fd = accept(listener, &conn_addr, &conn_addr_size);

		handle_connection(connected_fd);
	}

	close(listener);
	return 0;
}
