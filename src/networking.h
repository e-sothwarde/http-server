#define METHOD_SIZE 10
#define TARGET_SIZE 50
#define VERSION_SIZE 9
#define STATUS_SIZE 4
#define REASON_SIZE 50

struct fields;

struct request_line { // size 68
	char method[METHOD_SIZE];
	char target[TARGET_SIZE];
	char version[VERSION_SIZE];
};

struct request {
	struct request_line req_line;
	struct fields *field_lines;
	char *body;
};

struct status_line { // 61
	char version[VERSION_SIZE];
	char status[STATUS_SIZE];
	char reason_phrase[REASON_SIZE];
};

struct status {
	struct status_line stat_line;
	struct fields *field_lines;
	char *body;
};

// returns bound and listening socket
int listener_init(char *ip, int port, int backlog);

// parses recv buffer into request struct
int parse_request(char *recv_buf, struct request *req);

// prints request line
int print_request(struct request *req);

// formats and sends status message
int send_status(int sockfd, struct status *stat);

void handle_connection(int connected_fd);
