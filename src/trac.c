#include "cmd_options.h"
#include "tracdir.h"

int main(int argc, char ** argv) {
   // parse command line options
   cmd_options_t options = parse_command_line_options(argc, argv);
   
   // create the trac directory
   if (options.tracpath == NULL) {
      set_custrom_tracpath(get_default_tracpath());
   } else {
      set_custrom_tracpath(options.tracpath);
   }
   create_tracpath_dir();

   free_tracpath();
   return 0;
}
