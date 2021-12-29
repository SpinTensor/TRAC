#ifndef STORE_HISTORY_H
#define STORE_HISTORY_H

#include <stdio.h>

#include "tasks.h"
#include "segments.h"

void store_history(char *filename, trac_task_tree_t *task_tree, trac_segment_tree_t *segment_tree);

void store_history_header(FILE *filehandle);

void store_history_tasks(FILE *filehandle, trac_task_tree_t *task_tree);

void store_history_segments(FILE *filehandle, trac_segment_tree_t *segment_tree);

#endif
