#ifndef TASKS_H
#define TASKS_H

#include <stdbool.h>

typedef struct trac_task_type trac_task_t;
struct trac_task_type {
   int idx;
   bool valid;
   char *name;
   int level;
   int parentTaskidx;
   int nchildTasks;
   int *childTaskidxs;
};

typedef struct trac_task_tree_type trac_task_tree_t;
struct trac_task_tree_type {
   int ntasks;
   trac_task_t *tasks;
};

trac_task_tree_t new_task_tree();

int new_task(trac_task_tree_t *task_tree, int parentTaskidx, char *name);
int new_toplevel_task(trac_task_tree_t *task_tree, char *name);

void remove_task(trac_task_tree_t *task_tree, int idx);
void free_task_tree(trac_task_tree_t *task_tree);

#ifdef _DEBUG
void print_indent(int level);
void print_task_branch(trac_task_t *tasks, int idx);
void print_task_tree(trac_task_tree_t task_tree);
#endif

#endif
