struct request_line { // size 68
	char *method;
	char *target;
	char *version;
};

struct request {
	struct request_line req_line;
	struct fields *field_lines;
	char *body;
};

struct status_line { 
	char *version;
	char *status;
	char *reason_phrase;
};

/*
struct field {
	char field_str[64];
	struct fields *next_field;
};
*/

struct status { // 1152
	struct status_line stat_line;
	char *field_str;
	char *body;
};

int init_request(struct request *req);

int init_status(struct status *stat);

int get_str_size(char *str); // returns length of string in bytes

int set_str(char **dest, char *src); // resizes dest and copies src into it

// parses recv buffer into request struct
int parse_request(char *recv_buf, struct request_line *rl);

// prints request line
int print_request(struct request *req);

// prints status line
int print_status(struct status *stat);

void free_request(struct request *req);

void free_status(struct status *stat);
