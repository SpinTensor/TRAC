#ifndef CMD_OPT_TYPES_H
#define CMD_OPT_TYPES_H

#include <argp.h>

typedef struct {
   char *tracpath;
} cmd_options_t;

enum cmd_opt_IDs {
   tracpath_ID = 1024,
};

#endif
