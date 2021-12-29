#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <string.h>

#include "config.h"

#include "tasks.h"
#include "segments.h"
#include "timestamps.h"
#include "load_history.h"

void load_history(char *filename, trac_task_tree_t *task_tree, trac_segment_tree_t *segment_tree) {
   FILE *filehandle = fopen(filename, "r");
 
   load_history_header(filehandle);
 
   load_history_tasks(filehandle, task_tree);
 
   load_history_segments(filehandle, segment_tree);
 
   fclose(filehandle);
}

void load_history_header(FILE *filehandle) {
   (void) filehandle;
}

void load_history_tasks(FILE *filehandle, trac_task_tree_t *task_tree) {
   // number of tasks to be read
   int ntasks = 0;
   fread(&ntasks, sizeof(int), 1, filehandle);
   task_tree->ntasks = ntasks;
   task_tree->tasks = (trac_task_t*) malloc(ntasks*sizeof(trac_task_t));
   for (int itask=0; itask<ntasks; itask++) {
      trac_task_t *task = task_tree->tasks+itask;
      // set index
      task->idx = itask;
      // validity of the task
      fread(&(task->valid), sizeof(bool), 1, filehandle);
      // only read the rest of the required info if the entry is a valid task
      if (task->valid) {
         // name string
         int namelen;
         fread(&namelen, sizeof(int), 1, filehandle);
         task->name = (char*) malloc(namelen*sizeof(char));
         fread(task->name, sizeof(char), namelen, filehandle);
         // level
         fread(&(task->level), sizeof(int), 1, filehandle);
         // child task array
         fread(&(task->nchildTasks), sizeof(int), 1, filehandle);
         if (task->nchildTasks > 0) {
            task->childTaskidxs = (int*) malloc(task->nchildTasks*sizeof(int));
            fread(task->childTaskidxs, sizeof(int), task->nchildTasks, filehandle);
         } else {
            task->childTaskidxs = NULL;
         }
         // exclusive time (inclusive time can be computed from this)
         fread(&(task->excl_time), sizeof(int), 1, filehandle);

         // set the correct parent task index
         for (int ichild=0; ichild<task->nchildTasks; ichild++) {
            int childidx = task->childTaskidxs[ichild];
            task_tree->tasks[childidx].parentTaskidx = itask;
         }

      } else {
         // fill with dummy values
         task->name = NULL;
         task->level = -1;
         task->parentTaskidx = -1;
         task->nchildTasks = 0;
         task->childTaskidxs = NULL;
         task->incl_time = 0;
         task->excl_time = 0;
      }

   }

   // calculate the inclusive times
   calc_incl_time_tree(task_tree);
}

void load_history_segments(FILE *filehandle, trac_segment_tree_t *segment_tree) {
   // number of years in the segment tree
   int nyears = 0;
   fread(&nyears, sizeof(int), 1, filehandle);
   segment_tree->nyears = nyears;
   segment_tree->years = (trac_year_t*) malloc(nyears*sizeof(trac_year_t));
   for (int iyear=0; iyear<nyears; iyear++) {
      trac_year_t *year = segment_tree->years+iyear;
      // the year number
      fread(&(year->year_number), sizeof(int), 1, filehandle);
      // number of month in the year
      int nmonths = 0;
      fread(&nmonths, sizeof(int), 1, filehandle);
      year->nmonths = nmonths;
      year->months = (trac_month_t*) malloc(nmonths*sizeof(trac_month_t));
      for (int imonth=0; imonth<nmonths; imonth++) {
         trac_month_t *month = year->months+imonth;
         // the month in the year
         fread(&(month->month_of_year), sizeof(int), 1, filehandle);
         // number of days in the month
         int ndays = 0;
         fread(&ndays, sizeof(int), 1, filehandle);
         month->ndays = ndays;
         month->days = (trac_day_t*) malloc(ndays*sizeof(trac_day_t));
         for (int iday=0; iday<ndays; iday++) {
            trac_day_t *day = month->days+iday;
            // the day in the month
            fread(&(day->day_of_month), sizeof(int), 1, filehandle);
            // number of segments
            int nsegments = 0;
            fread(&nsegments, sizeof(int), 1, filehandle);
            day->nsegments = nsegments;
            day->segments = (trac_segment_t*) malloc(nsegments*sizeof(trac_segment_t));
            for (int isegment=0; isegment<nsegments; isegment++) {
               trac_segment_t *segment = day->segments+isegment;
               // task index
               fread(&(segment->taskidx), sizeof(int), 1, filehandle);
               // start and end time of segment. (just the seconds since epoch. rest can be constructed from that)
               unsigned long long starttime;
               fread(&starttime, sizeof(unsigned long long), 1, filehandle);
               segment->starttime = set_timestamp_seconds_since_epoch(starttime);
               unsigned long long endtime;
               fread(&endtime, sizeof(unsigned long long), 1, filehandle);
               segment->endtime = set_timestamp_seconds_since_epoch(endtime);
               segment->time = endtime - starttime;
            }
         }
      }
   }
}
