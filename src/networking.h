#include "http.h"

// returns bound and listening socket
int listener_init(char *ip, int port, int backlog);

// parses recv buffer into request struct
int parse_request(char *recv_buf, struct request *req);

// prints request line
int print_request(struct request *req);

// prints status line
int print_status(struct status *stat);

// formats and sends status message
int send_status(int sockfd, struct status *stat);

// receive data from client
void handle_connection(int connected_fd);
