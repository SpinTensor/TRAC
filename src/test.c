#include <stdlib.h>
#include <stdio.h>

#include "tasks.h"
#include "timestamps.h"
#include "segments.h"
#include "store_history.h"
#include "load_history.h"

int main(int argc, char **argv) {

   char mode = '\0';
   if (argc > 1) {
      mode = argv[1][0];
      if (mode != 'r' && mode != 'w') {
         return 1;
      }
   } else {
      return 1;
   }
   printf("Mode: %c\n\n", mode);

   trac_task_tree_t task_tree = new_task_tree();
   trac_segment_tree_t segment_tree = new_segment_tree();

   if (mode == 'w') {
      int top1idx = new_toplevel_task(&task_tree, "top1");
      int top2idx = new_toplevel_task(&task_tree, "top2");
      int top3idx = new_toplevel_task(&task_tree, "top3");

      int child1idx = new_task(&task_tree, top2idx, "child1");
      int child2idx = new_task(&task_tree, top2idx, "child2");
      int child3idx = new_task(&task_tree, top3idx, "child3");
      int child4idx = new_task(&task_tree, top1idx, "child4");
      int child5idx = new_task(&task_tree, child1idx, "child5");

      new_segment_current_timestamp(&segment_tree,0);
      trac_timestamp_t ts;
      for (int i=0; i<10; i++) {
         int year = 2000 + (rand()*3)%3;
         int month = rand()%(12+1);
         int day = rand()%(28+1);
         ts = set_timestamp_date(year, month, day, 12, 32, 1);
         int task = rand()%6;
         new_segment(&segment_tree, task, ts);
      }
      for (int i=0; i<10; i++) {
         ts = set_timestamp_date(2021, 12, 25, (12+17*i)%24, 32, 1);
         new_segment(&segment_tree, 5, ts);
      }
      ts = set_timestamp_date(2021, 12, 25, 20, 32, 1);
      new_segment(&segment_tree, 5, ts);
      ts = set_timestamp_date(2021, 12, 23, 20, 32, 1);
      new_segment(&segment_tree, 5, ts);
      remove_segment(&segment_tree, 2021, 12, 25, 1);
      
      store_history("asdf.out", &task_tree, &segment_tree);
   } else if (mode == 'r') {
      load_history("asdf.out", &task_tree, &segment_tree);
   }
   print_task_tree(stderr, task_tree);
   print_segment_tree(stderr, segment_tree);

   free_segment_tree(&segment_tree);
   free_task_tree(&task_tree);

   return 0;
}
