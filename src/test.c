#include "tasks.h"

int main(int argc, char **argv) {
   trac_task_list_t task_list = new_task_list();
   new_toplevel_task(&task_list, "top1");
   new_toplevel_task(&task_list, "top2");
   new_toplevel_task(&task_list, "top3");

   new_task(&task_list, 1, "child1");
   new_task(&task_list, 1, "child2");
   new_task(&task_list, 3, "child3");
   new_task(&task_list, 0, "child4");


   remove_task(&task_list, 3);

   print_task_tree(task_list);
   free_task_list(&task_list);
   return 0;
}
