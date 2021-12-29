#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _DEBUG
#include <stdio.h>
#endif

#include "tasks.h"

trac_task_tree_t new_task_tree() {
   trac_task_tree_t task_tree;
   task_tree.ntasks = 0;
   task_tree.tasks = NULL;
   return task_tree;
}

int new_task(trac_task_tree_t *task_tree, int parentTaskidx, char *name) {
   // search for a free spot
   int newTaskidx = -1;
   bool free_task = false;
   while (free_task == false && (newTaskidx+1)<task_tree->ntasks) {
      newTaskidx++;
      free_task = (!task_tree->tasks[newTaskidx].valid);
   }
   // if no free spot was found reallocate
   if (!free_task) {
      newTaskidx = task_tree->ntasks;
      task_tree->ntasks++;
      task_tree->tasks = (trac_task_t*) realloc(task_tree->tasks, task_tree->ntasks*sizeof(trac_task_t));
   }

   // fill in the new task info
   task_tree->tasks[newTaskidx].idx = newTaskidx;
   task_tree->tasks[newTaskidx].valid = true;
   task_tree->tasks[newTaskidx].name = strdup(name);
   task_tree->tasks[newTaskidx].parentTaskidx = parentTaskidx;
   task_tree->tasks[newTaskidx].incl_time = 0;
   task_tree->tasks[newTaskidx].excl_time = 0;
   if (parentTaskidx < 0) {
      task_tree->tasks[newTaskidx].level = 0;
   } else {
      task_tree->tasks[newTaskidx].level = task_tree->tasks[parentTaskidx].level + 1;
      // register as child task of parent task
      task_tree->tasks[parentTaskidx].nchildTasks++;
      task_tree->tasks[parentTaskidx].childTaskidxs = (int*)
         realloc(task_tree->tasks[parentTaskidx].childTaskidxs, task_tree->tasks[parentTaskidx].nchildTasks*sizeof(int));
      task_tree->tasks[parentTaskidx].childTaskidxs[task_tree->tasks[parentTaskidx].nchildTasks-1] = newTaskidx;
   }
   task_tree->tasks[newTaskidx].nchildTasks = 0;
   task_tree->tasks[newTaskidx].childTaskidxs = NULL;
   return newTaskidx;
}

int new_toplevel_task(trac_task_tree_t *task_tree, char *name) {
   return new_task(task_tree, -1, name);
}

void calc_incl_time(trac_task_t *tasks, int idx) {
   tasks[idx].incl_time = tasks[idx].excl_time;
   if (tasks[idx].nchildTasks > 0) {
      tasks[idx].incl_time = 0;
      for (int itask=0; itask<tasks->nchildTasks; itask++) {
         int childTaskidx = tasks->childTaskidxs[itask];
         if (tasks[childTaskidx].valid) {
            calc_incl_time(tasks, childTaskidx);
            tasks[idx].incl_time += tasks[childTaskidx].incl_time;
         }
      }
   }
}

void calc_incl_time_tree(trac_task_tree_t *task_tree) {
   for (int itask=0; itask<task_tree->ntasks; itask++) {
      if (task_tree->tasks[itask].level == 0 && task_tree->tasks[itask].valid) {
         calc_incl_time(task_tree->tasks, itask);
      }
   }
}

void remove_task(trac_task_tree_t *task_tree, int idx) {
   trac_task_t *task = task_tree->tasks+idx;
   remove_all_child_tasks(task_tree, idx);
   if (task->level > 0) {
      // remove entry from parent child list
      task_tree->tasks[task->parentTaskidx].nchildTasks--;
      for (int itask=idx; itask<task_tree->tasks[task->parentTaskidx].nchildTasks; itask++) {
         task_tree->tasks[task->parentTaskidx].childTaskidxs[itask] =
            task_tree->tasks[task->parentTaskidx].childTaskidxs[itask+1];
      }
   }
}

void remove_all_child_tasks(trac_task_tree_t *task_tree, int idx) {
   trac_task_t *task = task_tree->tasks+idx;
   if (task->nchildTasks > 0) {
      // recursively remove all child tasks and their children
      for (int itask=0; itask<task->nchildTasks; itask++) {
         remove_all_child_tasks(task_tree, task->childTaskidxs[itask]);
      }
      free(task->childTaskidxs);
      task->childTaskidxs = NULL;
      task->nchildTasks = 0;
   }
   task->valid = false;
   free(task->name);
   task->name = NULL;
}

void free_task_tree(trac_task_tree_t *task_tree) {
   for (int itask=0; itask<task_tree->ntasks; itask++) {
      if (task_tree->tasks[itask].level == 0 && task_tree->tasks[itask].valid) {
         remove_task(task_tree, itask);
      }
   }
   free(task_tree->tasks);
   task_tree->tasks = NULL;
   task_tree->ntasks = 0;

}

#ifdef _DEBUG
void print_indent(FILE *io_handle, int level) {
   for (int ilevel=0; ilevel<level; ilevel++) {
      fprintf(io_handle, "   ");
   }
}

void print_task_branch(FILE *io_handle, trac_task_t *tasks, int idx) {
   print_indent(io_handle, tasks[idx].level);
   fprintf(io_handle, "%d: %s\n", tasks[idx].idx, tasks[idx].name);
   if (tasks[idx].nchildTasks > 0) {
      for (int itask=0; itask<tasks[idx].nchildTasks; itask++) {
         print_task_branch(io_handle, tasks, tasks[idx].childTaskidxs[itask]);
      }
   }
}

void print_task_tree(FILE *io_handle, trac_task_tree_t task_tree) {
   for (int itask=0; itask<task_tree.ntasks; itask++) {
      if (task_tree.tasks[itask].level == 0 && task_tree.tasks[itask].valid) {
         print_task_branch(io_handle, task_tree.tasks, itask);
      }
   }
}

#endif
