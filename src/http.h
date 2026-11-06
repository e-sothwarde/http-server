#define METHOD_SIZE 10
#define TARGET_SIZE 50
#define VERSION_SIZE 9
#define STATUS_SIZE 4
#define REASON_SIZE 50
#define STAT_LINE_SIZE = 63

#include "structs.h"

int get(struct request *req, struct status *stat);

// calls requested http method
int select_method(struct request *req, struct status *stat);
