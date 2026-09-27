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

int get(struct request *req, struct status *stat);
int handle_request(struct request *req, struct status *stat);
