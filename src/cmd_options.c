#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <limits.h>
#include <argp.h>

#include <config.h>

#include "cmd_opt_types.h"

const char *argp_program_version = PACKAGE_VERSION;

const char *argp_program_bug_address = PACKAGE_BUGREPORT;

/* Program documentation. */
static char doc[] = "Time Recorder and Activity Controler";

// A description of the arguments we accept.
// static char args_doc[] = "";

// the possible program options
static struct argp_option possible_options[] = {
   {
      "trac-dir",
      tracpath_ID,
      "t",
      0,
      "Directory to store config and history (Default: ~/.trac/)",
      0
   }, {
      0, 0, 0, 0, 0, 0
   }
};

// Option parsing function
static error_t parse_cmd_options(int key, char *arg, struct argp_state *state) {
   // Get the input argument from argp_parse, which we
   // know is a pointer to our arguments structure. */
   cmd_options_t *options = state->input;

   switch (key) {
      // Intervall amounts
      case tracpath_ID:
         options->tracpath = arg;
         break;
      case ARGP_KEY_ARG:
         if (state->arg_num > 0) {
            argp_usage(state);
         }
         break;
      default:
         return ARGP_ERR_UNKNOWN;
         break;
   }

   return 0;
}

// Our argp parser.
static struct argp argp = {possible_options,
                           parse_cmd_options,
                           NULL,
                           doc,
                           NULL, NULL, NULL};

// easy interface to command line option parsing
cmd_options_t parse_command_line_options(int argc, char **argv) {
   cmd_options_t options;
   options.tracpath = NULL;

   // actually parse the arguments
   argp_parse(&argp, argc, argv, 0, 0, &options);

   return options;
}
