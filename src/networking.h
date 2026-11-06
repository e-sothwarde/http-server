#include "structs.h"

// returns bound and listening socket
int listener_init(char *ip, int port, int backlog);

// formats and sends status message
int send_status(int sockfd, struct status *stat);

// receive data from client - this should be in main?
void handle_connection(int connected_fd);
