#ifndef TASKS_H
#define TASKS_H

#include <stdbool.h>

#ifdef _DEBUG
#include <stdio.h>
#endif

typedef struct trac_task_type trac_task_t;
struct trac_task_type {
   int idx;
   bool valid;
   char *name;
   int level;
   int parentTaskidx;
   int nchildTasks;
   int *childTaskidxs;
   int incl_time;
   int excl_time;
};

typedef struct trac_task_tree_type trac_task_tree_t;
struct trac_task_tree_type {
   int ntasks;
   trac_task_t *tasks;
};

trac_task_tree_t new_task_tree();

int new_task(trac_task_tree_t *task_tree, int parentTaskidx, char *name);
int new_toplevel_task(trac_task_tree_t *task_tree, char *name);

void calc_incl_time(trac_task_tree_t *task_tree, int idx);
void calc_incl_time_tree(trac_task_tree_t *task_tree);

void remove_task(trac_task_tree_t *task_tree, int idx);
void free_task_tree(trac_task_tree_t *task_tree);

#ifdef _DEBUG
void print_indent(FILE *io_handle, int level);
void print_task_branch(FILE *io_handle, trac_task_t *tasks, int idx);
void print_task_tree(FILE *io_handle, trac_task_tree_t task_tree);
#endif

#endif
