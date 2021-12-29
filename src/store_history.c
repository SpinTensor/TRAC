#include <stdio.h>
#include <stdbool.h>

#include <string.h>

#include "config.h"

#include "tasks.h"
#include "segments.h"
#include "timestamps.h"
#include "store_history.h"

void store_history(char *filename, trac_task_tree_t *task_tree, trac_segment_tree_t *segment_tree) {
   FILE *filehandle = fopen(filename, "w");

   store_history_header(filehandle);

   store_history_tasks(filehandle, task_tree);

   store_history_segments(filehandle, segment_tree);

   fclose(filehandle);
}

void store_history_header(FILE *filehandle) {
   (void) filehandle;
}

void store_history_tasks(FILE *filehandle, trac_task_tree_t *task_tree) {
   // number of tasks to be written.
   int ntasks = task_tree->ntasks;
   fwrite(&ntasks, sizeof(int), 1, filehandle);
   for (int itask=0; itask<ntasks; itask++) {
      trac_task_t *task = task_tree->tasks+itask;
      // validity of the task memory
      fwrite(&(task->valid), sizeof(bool), 1, filehandle);
      // only write the rest of the required info if the entry is a valid task
      if (task->valid) {
         // name string
         int namelen = strlen(task->name) + 1;
         fwrite(&namelen, sizeof(int), 1, filehandle);
         fwrite(task->name, sizeof(char), namelen, filehandle);
         // level
         fwrite(&(task->level), sizeof(int), 1, filehandle);
         // child task array
         fwrite(&(task->nchildTasks), sizeof(int), 1, filehandle);
         if (task->nchildTasks > 0) {
            fwrite(task->childTaskidxs, sizeof(int), task->nchildTasks, filehandle);
         }
         // exclusive time (inclusive time can be computed from this)
         fwrite(&(task->excl_time), sizeof(int), 1, filehandle);
      }
   }
}

void store_history_segments(FILE *filehandle, trac_segment_tree_t *segment_tree) {
   // number of years in the segment tree
   int nyears = segment_tree->nyears;
   fwrite(&nyears, sizeof(int), 1, filehandle);
   for (int iyear=0; iyear<nyears; iyear++) {
      trac_year_t *year = segment_tree->years+iyear;
      // the year number
      fwrite(&(year->year_number), sizeof(int), 1, filehandle);
      // number of month in the year
      int nmonths = year->nmonths;
      fwrite(&nmonths, sizeof(int), 1, filehandle);
      for (int imonth=0; imonth<nmonths; imonth++) {
         trac_month_t *month = year->months+imonth;
         // the month in the year
         fwrite(&(month->month_of_year), sizeof(int), 1, filehandle);
         // number of days in the month
         int ndays = month->ndays;
         fwrite(&ndays, sizeof(int), 1, filehandle);
         for (int iday=0; iday<ndays; iday++) {
            trac_day_t *day = month->days+iday;
            // the day in the month
            fwrite(&(day->day_of_month), sizeof(int), 1, filehandle);
            // number of segments
            int nsegments = day->nsegments;
            fwrite(&nsegments, sizeof(int), 1, filehandle);
            for (int isegment=0; isegment<nsegments; isegment++) {
               trac_segment_t *segment = day->segments+isegment;
               // task index
               fwrite(&(segment->taskidx), sizeof(int), 1, filehandle);
               // start and end time of segment. (just the seconds since epoch. rest can be constructed from that)
               unsigned long long starttime = get_timestamp_seconds_since_epoch(segment->starttime);
               fwrite(&starttime, sizeof(unsigned long long), 1, filehandle);
               unsigned long long endtime = get_timestamp_seconds_since_epoch(segment->endtime);
               fwrite(&endtime, sizeof(unsigned long long), 1, filehandle);
            }
         }
      }
   }
}
