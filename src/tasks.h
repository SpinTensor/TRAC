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

typedef struct trac_task_list_type trac_task_list_t;
struct trac_task_list_type {
   int ntasks;
   trac_task_t *tasks;
};

trac_task_list_t new_task_list();

int new_task(trac_task_list_t *task_list, int parentTaskidx, char *name);
int new_toplevel_task(trac_task_list_t *task_list, char *name);

void remove_task(trac_task_list_t *task_list, int idx);
void free_task_list(trac_task_list_t *task_list);

#ifdef _DEBUG
void print_task(trac_task_t task);
void print_task_list(trac_task_list_t task_list);

void print_indent(int level);
void print_task_branch(trac_task_t *tasks, int idx);
void print_task_tree(trac_task_list_t task_list);
#endif

#endif
