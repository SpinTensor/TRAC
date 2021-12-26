#include <stdlib.h>
#include <stdio.h>

#include "tasks.h"
#include "timestamps.h"
#include "segments.h"

int main(int argc, char **argv) {
   trac_task_tree_t task_tree = new_task_tree();
   new_toplevel_task(&task_tree, "top1");
   new_toplevel_task(&task_tree, "top2");
   new_toplevel_task(&task_tree, "top3");

   new_task(&task_tree, 1, "child1");
   new_task(&task_tree, 1, "child2");
   new_task(&task_tree, 3, "child3");
   new_task(&task_tree, 0, "child4");


   print_task_tree(stderr, task_tree);

   trac_timestamp_t ts = get_current_timestamp();
   fprintf(stderr, "\n");
   print_timestamp_seconds_since_epoch(stderr, ts); fprintf(stderr, "\n");
   print_timestamp_date(stderr, ts); fprintf(stderr, "\n");
   unsigned long long s = get_timestamp_seconds_since_epoch(ts);
   s += 60*60*24;
   ts = set_timestamp_seconds_since_epoch(s);
   print_timestamp_seconds_since_epoch(stderr, ts); fprintf(stderr, "\n");
   print_timestamp_date(stderr, ts); fprintf(stderr, "\n");

   ts = set_timestamp_date(2021, 12, 25, 12, 32, 1);
   print_timestamp_seconds_since_epoch(stderr, ts); fprintf(stderr, "\n");
   print_timestamp_date(stderr, ts); fprintf(stderr, "\n");

   trac_segment_tree_t segment_tree = new_segment_tree();
   new_segment_current_timestamp(&segment_tree,0);
   for (int i=0; i<10; i++) {
      int year = 2021;
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
   print_segment_tree(stderr, segment_tree);

   free_segment_tree(&segment_tree);
   free_task_tree(&task_tree);

   return 0;
}
