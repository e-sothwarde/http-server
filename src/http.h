#define METHOD_SIZE 10
#define TARGET_SIZE 50
#define VERSION_SIZE 9
#define STATUS_SIZE 4
#define REASON_SIZE 50
#define STAT_LINE_SIZE = 63

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

struct status_line { 
	char version[VERSION_SIZE];
	char status[STATUS_SIZE];
	char reason_phrase[REASON_SIZE];
};

/*
struct field {
	char field_str[64];
	struct fields *next_field;
};
*/

struct status { // 1152
	struct status_line stat_line;
	char field_str[64];
	char body[512];
};

// parses recv buffer into request struct
int parse_request(char *recv_buf, struct request *req);

// prints request line
int print_request(struct request *req);

// prints status line
int print_status(struct status *stat);

int get(struct request *req, struct status *stat);

// calls requested http method
int select_method(struct request *req, struct status *stat);
