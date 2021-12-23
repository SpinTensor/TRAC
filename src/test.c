#include <stdio.h>

#include "tasks.h"
#include "timestamps.h"

int main(int argc, char **argv) {
   trac_task_tree_t task_tree = new_task_tree();
   new_toplevel_task(&task_tree, "top1");
   new_toplevel_task(&task_tree, "top2");
   new_toplevel_task(&task_tree, "top3");

   new_task(&task_tree, 1, "child1");
   new_task(&task_tree, 1, "child2");
   new_task(&task_tree, 3, "child3");
   new_task(&task_tree, 0, "child4");


   remove_task(&task_tree, 3);

   print_task_tree(task_tree);
   free_task_tree(&task_tree);

   trac_timestamp_t ts = get_current_timestamp();
   printf("\n");
   printf("%lld\n", get_timestamp_seconds_since_epoch(ts));
   printf("%d:%d:%d %d.%d.%d\n", get_timestamp_hour(ts), get_timestamp_minute(ts), get_timestamp_second(ts), get_timestamp_day(ts), get_timestamp_month(ts), get_timestamp_year(ts));

   unsigned long long s = get_timestamp_seconds_since_epoch(ts);
   s += 60*60*24;
   ts = set_timestamp_seconds_since_epoch(s);
   printf("%lld\n", get_timestamp_seconds_since_epoch(ts));
   printf("%d:%d:%d %d.%d.%d\n", get_timestamp_hour(ts), get_timestamp_minute(ts), get_timestamp_second(ts), get_timestamp_day(ts), get_timestamp_month(ts), get_timestamp_year(ts));

   ts = set_timestamp_date(2021, 12, 25, 12, 32, 1);
   printf("%lld\n", get_timestamp_seconds_since_epoch(ts));
   printf("%d:%d:%d %d.%d.%d\n", get_timestamp_hour(ts), get_timestamp_minute(ts), get_timestamp_second(ts), get_timestamp_day(ts), get_timestamp_month(ts), get_timestamp_year(ts));


   return 0;
}
