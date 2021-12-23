#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _DEBUG
#include <stdio.h>
#endif

#include "tasks.h"

trac_task_list_t new_task_list() {
   trac_task_list_t task_list;
   task_list.ntasks = 0;
   task_list.tasks = NULL;
   return task_list;
}

int new_task(trac_task_list_t *task_list, int parentTaskidx, char *name) {
   // search for a free spot
   int newTaskidx = -1;
   bool free_task = false;
   while (free_task == false && (newTaskidx+1)<task_list->ntasks) {
      newTaskidx++;
      free_task = (!task_list->tasks[newTaskidx].valid);
   }
   // if no free spot was found reallocate
   if (!free_task) {
      newTaskidx = task_list->ntasks;
      task_list->ntasks++;
      task_list->tasks = (trac_task_t*) realloc(task_list->tasks, task_list->ntasks*sizeof(trac_task_t));
   }

   // fill in the new task info
   task_list->tasks[newTaskidx].idx = newTaskidx;
   task_list->tasks[newTaskidx].valid = true;
   task_list->tasks[newTaskidx].name = strdup(name);
   task_list->tasks[newTaskidx].parentTaskidx = parentTaskidx;
   if (parentTaskidx < 0) {
      task_list->tasks[newTaskidx].level = 0;
   } else {
      task_list->tasks[newTaskidx].level = task_list->tasks[parentTaskidx].level + 1;
      // register as child task of parent task
      task_list->tasks[parentTaskidx].nchildTasks++;
      task_list->tasks[parentTaskidx].childTaskidxs = (int*)
         realloc(task_list->tasks[parentTaskidx].childTaskidxs, task_list->tasks[parentTaskidx].nchildTasks*sizeof(int));
      task_list->tasks[parentTaskidx].childTaskidxs[task_list->tasks[parentTaskidx].nchildTasks-1] = newTaskidx;
   }
   task_list->tasks[newTaskidx].nchildTasks = 0;
   task_list->tasks[newTaskidx].childTaskidxs = NULL;
   return newTaskidx;
}

int new_toplevel_task(trac_task_list_t *task_list, char *name) {
   return new_task(task_list, -1, name);
}

void remove_task(trac_task_list_t *task_list, int idx) {
   trac_task_t *task = task_list->tasks+idx;
   if (task->nchildTasks > 0) {
      // recursively remove all child tasks and their children
      for (int itask=0; itask<task->nchildTasks; itask++) {
         remove_task(task_list, task->childTaskidxs[itask]);
      }
      free(task->childTaskidxs);
      task->childTaskidxs = NULL;
      task->nchildTasks = 0;
   }
   task->valid = false;
   free(task->name);
   task->name = NULL;
   if (task->level > 0) {
      // remove entry from parent child list
      int offset = 0;
      task_list->tasks[task->parentTaskidx].nchildTasks--;
      for (int itask=0; itask<task_list->tasks[task->parentTaskidx].nchildTasks; itask++) {
         if (task_list->tasks[task->parentTaskidx].childTaskidxs[itask] == idx) {
            offset++;
         }
         task_list->tasks[task->parentTaskidx].childTaskidxs[itask] =
            task_list->tasks[task->parentTaskidx].childTaskidxs[itask+offset];
      }
   }
}

void free_task_list(trac_task_list_t *task_list) {
   for (int itask=0; itask<task_list->ntasks; itask++) {
      if (task_list->tasks[itask].level == 0 && task_list->tasks[itask].valid) {
         remove_task(task_list, itask);
      }
   }
   free(task_list->tasks);
   task_list->tasks = NULL;
   task_list->ntasks = 0;

}

#ifdef _DEBUG
void print_task(trac_task_t task) {
   printf("name: %s\n", task.name);
   printf("idx: %d\n", task.idx);
   printf("level: %d\n", task.level);
   printf("parenttaskidx: %d\n", task.parentTaskidx);
   printf("nchildTasks: %d\n", task.nchildTasks);
   if (task.nchildTasks > 0) {
      printf("childTaskidxs:");
      for (int itask=0; itask<task.nchildTasks; itask++) {
         printf(" %d", task.childTaskidxs[itask]);
      }
      printf("\n");
   }
   printf("\n");
}

void print_task_list(trac_task_list_t task_list) {
   for (int itask=0; itask<task_list.ntasks; itask++) {
      if (task_list.tasks[itask].valid) {
         print_task(task_list.tasks[itask]);
      }
   }
}

void print_indent(int level) {
   for (int ilevel=0; ilevel<level; ilevel++) {
      printf("   ");
   }
}

void print_task_branch(trac_task_t *tasks, int idx) {
   print_indent(tasks[idx].level);
   printf("name: %s\n", tasks[idx].name);
   print_indent(tasks[idx].level);
   printf("idx: %d\n", tasks[idx].idx);
   print_indent(tasks[idx].level);
   printf("level: %d\n", tasks[idx].level);
   print_indent(tasks[idx].level);
   printf("parenttaskidx: %d\n", tasks[idx].parentTaskidx);
   print_indent(tasks[idx].level);
   printf("nchildTasks: %d\n", tasks[idx].nchildTasks);
   if (tasks[idx].nchildTasks > 0) {
      print_indent(tasks[idx].level);
      printf("childTaskidxs:");
      for (int itask=0; itask<tasks[idx].nchildTasks; itask++) {
         printf(" %d", tasks[idx].childTaskidxs[itask]);
      }
      printf("\n");
      for (int itask=0; itask<tasks[idx].nchildTasks; itask++) {
         print_task_branch(tasks, tasks[idx].childTaskidxs[itask]);
      }
   }
}

void print_task_tree(trac_task_list_t task_list) {
   for (int itask=0; itask<task_list.ntasks; itask++) {
      if (task_list.tasks[itask].level == 0 && task_list.tasks[itask].valid) {
         print_task_branch(task_list.tasks, itask);
      }
   }
}

#endif
