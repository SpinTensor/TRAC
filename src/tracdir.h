#ifndef TRACDIR_H
#define TRACDIR_H

char *get_default_tracpath();

void *set_custrom_tracpath(char *custom_path);

char *get_current_tracpath();

void free_tracpath();

int create_tracpath_dir();

#endif
