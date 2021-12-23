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

int new_task(trac_task_list_t *task_list, int parentTaskID, char *name) {
   // search for a free spot
   int newTaskID = -1;
   bool free_task = false;
   while (free_task == false && (newTaskID+1)<task_list->ntasks) {
      newTaskID++;
      free_task = (!task_list->tasks[newTaskID].valid);
   }
   // if no free spot was found reallocate
   if (!free_task) {
      newTaskID = task_list->ntasks;
      task_list->ntasks++;
      task_list->tasks = (trac_task_t*) realloc(task_list->tasks, task_list->ntasks*sizeof(trac_task_t));
   }

   // fill in the new task info
   task_list->tasks[newTaskID].ID = newTaskID;
   task_list->tasks[newTaskID].valid = true;
   task_list->tasks[newTaskID].name = strdup(name);
   task_list->tasks[newTaskID].parentTaskID = parentTaskID;
   if (parentTaskID < 0) {
      task_list->tasks[newTaskID].level = 0;
   } else {
      task_list->tasks[newTaskID].level = task_list->tasks[parentTaskID].level + 1;
      // register as child task of parent task
      task_list->tasks[parentTaskID].nchildTasks++;
      task_list->tasks[parentTaskID].childTaskIDs = (int*)
         realloc(task_list->tasks[parentTaskID].childTaskIDs, task_list->tasks[parentTaskID].nchildTasks*sizeof(int));
      task_list->tasks[parentTaskID].childTaskIDs[task_list->tasks[parentTaskID].nchildTasks-1] = newTaskID;
   }
   task_list->tasks[newTaskID].nchildTasks = 0;
   task_list->tasks[newTaskID].childTaskIDs = NULL;
   return newTaskID;
}

int new_toplevel_task(trac_task_list_t *task_list, char *name) {
   return new_task(task_list, -1, name);
}

void remove_task(trac_task_list_t *task_list, int ID) {
   trac_task_t *task = task_list->tasks+ID;
   if (task->nchildTasks > 0) {
      // recursively remove all child tasks and their children
      for (int i=0; i<task->nchildTasks; i++) {
         remove_task(task_list, task->childTaskIDs[i]);
      }
      free(task->childTaskIDs);
      task->childTaskIDs = NULL;
      task->nchildTasks = 0;
   }
   task->valid = false;
   free(task->name);
   task->name = NULL;
   if (task->level > 0) {
      // remove entry from parent child list
      int offset = 0;
      task_list->tasks[task->parentTaskID].nchildTasks--;
      for (int i=0; i<task_list->tasks[task->parentTaskID].nchildTasks; i++) {
         if (task_list->tasks[task->parentTaskID].childTaskIDs[i] == ID) {
            offset++;
         }
         task_list->tasks[task->parentTaskID].childTaskIDs[i] = task_list->tasks[task->parentTaskID].childTaskIDs[i+offset];
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
   printf("ID: %d\n", task.ID);
   printf("level: %d\n", task.level);
   printf("parenttaskID: %d\n", task.parentTaskID);
   printf("nchildTasks: %d\n", task.nchildTasks);
   if (task.nchildTasks > 0) {
      printf("childTaskIDs:");
      for (int i=0; i<task.nchildTasks; i++) {
         printf(" %d", task.childTaskIDs[i]);
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

void print_task_branch(trac_task_t *tasks, int ID) {
   print_indent(tasks[ID].level);
   printf("name: %s\n", tasks[ID].name);
   print_indent(tasks[ID].level);
   printf("ID: %d\n", tasks[ID].ID);
   print_indent(tasks[ID].level);
   printf("level: %d\n", tasks[ID].level);
   print_indent(tasks[ID].level);
   printf("parenttaskID: %d\n", tasks[ID].parentTaskID);
   print_indent(tasks[ID].level);
   printf("nchildTasks: %d\n", tasks[ID].nchildTasks);
   if (tasks[ID].nchildTasks > 0) {
      print_indent(tasks[ID].level);
      printf("childTaskIDs:");
      for (int i=0; i<tasks[ID].nchildTasks; i++) {
         printf(" %d", tasks[ID].childTaskIDs[i]);
      }
      printf("\n");
      for (int ichild=0; ichild<tasks[ID].nchildTasks; ichild++) {
         print_task_branch(tasks, tasks[ID].childTaskIDs[ichild]);
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
