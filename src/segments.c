#include <stdlib.h>

#include "tasks.h"
#include "timestamps.h"
#include "segments.h"

#ifdef _DEBUG
#include <stdio.h>
#endif

trac_segment_tree_t new_segment_tree() {
   trac_segment_tree_t segment_tree;
   segment_tree.nyears = 0;
   segment_tree.years = NULL;
   return segment_tree;
}

trac_year_t *new_year(trac_segment_tree_t *segment_tree, int year_number) {
   segment_tree->nyears++;
   segment_tree->years = (trac_year_t*) realloc(segment_tree->years, segment_tree->nyears*sizeof(trac_year_t));
   trac_year_t *year = segment_tree->years+(segment_tree->nyears-1);
   year->year_number = year_number;
   year->nmonths = 0;
   year->months = NULL;
   // sort the year list (if at all, only the last element should be unsorted)
   for (int iyear=segment_tree->nyears-1; iyear>0; iyear--) {
      if (segment_tree->years[iyear-1].year_number <= segment_tree->years[iyear].year_number) {
         break;
      }
      trac_year_t tmpyear = segment_tree->years[iyear];
      segment_tree->years[iyear] = segment_tree->years[iyear-1];
      segment_tree->years[iyear-1] = tmpyear;
      year--;
   }
   return year;
}

trac_month_t *new_month(trac_year_t *year, int month_of_year) {
   year->nmonths++;
   year->months = (trac_month_t*) realloc(year->months, year->nmonths*sizeof(trac_month_t));
   trac_month_t *month = year->months+(year->nmonths-1);
   month->month_of_year = month_of_year;
   month->ndays = 0;
   month->days = NULL;
   // sort the month list (if at all, only the last element should be unsorted)
   for (int imonth=year->nmonths-1; imonth>0; imonth--) {
      if (year->months[imonth-1].month_of_year <= year->months[imonth].month_of_year) {
         break;
      }
      trac_month_t tmpmonth = year->months[imonth];
      year->months[imonth] = year->months[imonth-1];
      year->months[imonth-1] = tmpmonth;
      month--;
   }
   return month;
}

trac_day_t *new_day(trac_month_t *month, int day_of_month) {
   month->ndays++;
   month->days = (trac_day_t*) realloc(month->days, month->ndays*sizeof(trac_day_t));
   trac_day_t *day = month->days+(month->ndays-1);
   day->day_of_month = day_of_month;
   day->nsegments = 0;
   day->segments = NULL;
   // sort the day list (if at all, only the last element should be unsorted)
   for (int iday=month->ndays-1; iday>0; iday--) {
      if (month->days[iday-1].day_of_month < month->days[iday].day_of_month) {
         break;
      }
      trac_day_t tmpday = month->days[iday];
      month->days[iday] = month->days[iday-1];
      month->days[iday-1] = tmpday;
      day--;
   }
   return day;
}

trac_segment_t *new_segment(trac_segment_tree_t *segment_tree, int taskidx, trac_timestamp_t starttime) {
   // search whether the year already exists
   int year_number = get_timestamp_year(starttime);
   trac_year_t *year = NULL;
   for (int iyear=0; iyear<segment_tree->nyears; iyear++) {
      trac_year_t *tmpyear = segment_tree->years+iyear;
      if (year_number == tmpyear->year_number) {
         year = tmpyear;
         break;
      }
   }
   if (year == NULL) {
      year = new_year(segment_tree, year_number);
   }

   // search whether the month already exists
   int month_of_year = get_timestamp_month(starttime);
   trac_month_t *month = NULL;
   for (int imonth=0; imonth<year->nmonths; imonth++) {
      trac_month_t *tmpmonth = year->months+imonth;
      if (month_of_year == tmpmonth->month_of_year) {
         month = tmpmonth;
         break;
      }
   }
   if (month == NULL) {
      month = new_month(year, month_of_year);
   }

   // search whether the day already exists
   int day_of_month = get_timestamp_day(starttime);
   trac_day_t *day = NULL;
   for (int iday=0; iday<month->ndays; iday++) {
      trac_day_t *tmpday = month->days+iday;
      if (day_of_month == tmpday->day_of_month) {
         day = tmpday;
         break;
      }
   }
   if (day == NULL) {
      day = new_day(month, day_of_month);
   }

   // add the segment
   day->nsegments++;
   day->segments = (trac_segment_t*) realloc(day->segments, day->nsegments*sizeof(trac_segment_t));
   trac_segment_t *segment = day->segments+(day->nsegments-1);
   segment->taskidx = taskidx;
   segment->starttime = starttime;
   segment->endtime = starttime;
   segment->time = 0;
   // sort the segment list (if at all, only the last element should be unsorted)
   unsigned long long seconds_since_epoch = get_timestamp_seconds_since_epoch(segment->starttime);
   for (int isegment=day->nsegments-1; isegment>0; isegment--) {
      unsigned long long tmpseconds_since_epoch = get_timestamp_seconds_since_epoch(day->segments[isegment-1].starttime);
      if (tmpseconds_since_epoch <= seconds_since_epoch) {
         break;
      }
      trac_segment_t tmpsegment = day->segments[isegment];
      day->segments[isegment] = day->segments[isegment-1];
      day->segments[isegment-1] = tmpsegment;
      segment--;
   }

   return segment;
}

trac_segment_t *new_segment_current_timestamp(trac_segment_tree_t *segment_tree, int taskidx) {
   trac_timestamp_t starttime = get_current_timestamp();
   return new_segment(segment_tree, taskidx, starttime);
}

int get_year_idx_from_number(trac_segment_tree_t *segment_tree, int year_number) {
   int year_idx = -1;
   for (int iyear=0; iyear<segment_tree->nyears; iyear++) {
      if (year_number == segment_tree->years[iyear].year_number) {
         year_idx = iyear;
         break;
      }
   }
   return year_idx;
}

int get_month_idx_from_number(trac_year_t *year, int month_of_year) {
   int month_idx = -1;
   for (int imonth=0; imonth<year->nmonths; imonth++) {
      if (month_of_year == year->months[imonth].month_of_year) {
         month_idx = imonth;
         break;
      }
   }
   return month_idx;
}

int get_day_idx_from_number(trac_month_t *month, int day_of_month) {
   int day_idx = -1;
   for (int iday=0; iday<month->ndays; iday++) {
      if (day_of_month == month->days[iday].day_of_month) {
         day_idx = iday;
         break;
      }  
   }  
   return day_idx;
}

void remove_segment(trac_segment_tree_t *segment_tree, int year_number, int month_of_year, int day_of_month, int segment_idx) {
   int year_idx = get_year_idx_from_number(segment_tree, year_number);
   if (year_idx >= 0) {
     trac_year_t *year = segment_tree->years+year_idx;
     int month_idx = get_month_idx_from_number(year, month_of_year);
     if (month_idx >= 0) {
        trac_month_t *month = year->months+month_idx;
        int day_idx = get_day_idx_from_number(month, day_of_month);
        if (day_idx >= 0) {
           trac_day_t *day = month->days+day_idx;
           if (segment_idx >= 0 && segment_idx < day->nsegments) {
              free_segment(day->segments+segment_idx);
              day->nsegments--;
              for (int isegment=segment_idx; isegment<day->nsegments; isegment++) {
                 day->segments[isegment] = day->segments[isegment+1];
              }
           }
        }
     }
   }
}

void remove_day(trac_segment_tree_t *segment_tree, int year_number, int month_of_year, int day_of_month) {
   int year_idx = get_year_idx_from_number(segment_tree, year_number);
   if (year_idx >= 0) {
     trac_year_t *year = segment_tree->years+year_idx;
     int month_idx = get_month_idx_from_number(year, month_of_year);
     if (month_idx >= 0) {
        trac_month_t *month = year->months+month_idx;
        int day_idx = get_day_idx_from_number(month, day_of_month);
        if (day_idx >= 0) {
           free_day(month->days+day_idx);
           month->ndays--;
           for (int iday=day_idx; iday<month->ndays; iday++) {
              month->days[iday] = month->days[iday+1];
           }
        }
     }
   }
}

void remove_month(trac_segment_tree_t *segment_tree, int year_number, int month_of_year) {
   int year_idx = get_year_idx_from_number(segment_tree, year_number);
   if (year_idx >= 0) {
     trac_year_t *year = segment_tree->years+year_idx;
     int month_idx = get_month_idx_from_number(year, month_of_year);
     if (month_idx >= 0) {
        free_month(year->months+month_idx);
        year->nmonths--;
        for (int imonth=month_idx; imonth<year->nmonths; imonth++) {
           year->months[imonth] = year->months[imonth+1];
        }
     }
   }
}

void remove_year(trac_segment_tree_t *segment_tree, int year_number) {
   int year_idx = get_year_idx_from_number(segment_tree, year_number);
   if (year_idx >= 0) {
      free_year(segment_tree->years+year_idx);
      segment_tree->nyears--;
      for (int iyear=year_idx; iyear<segment_tree->nyears; iyear++) {
         segment_tree->years[iyear] = segment_tree->years[iyear+1];
      }
   }
}

void free_segment(trac_segment_t *segment) {
   (void) segment;
}

void free_day(trac_day_t *day) {
   for (int isegment=0; isegment<day->nsegments; isegment++) {
      free_segment(day->segments+isegment);
   }
   free(day->segments);
   day->segments = NULL;
   day->nsegments = 0;
}

void free_month(trac_month_t *month) {
   for (int iday=0; iday<month->ndays; iday++) {
      free_day(month->days+iday);
   }
   free(month->days);
   month->days = NULL;
   month->ndays = 0;
}

void free_year(trac_year_t *year) {
   for (int imonth=0; imonth<year->nmonths; imonth++) {
      free_month(year->months+imonth);
   }
   free(year->months);
   year->months = NULL;
   year->nmonths = 0;
}

void free_segment_tree(trac_segment_tree_t *segment_tree) {
   for (int iyear=0; iyear<segment_tree->nyears; iyear++) {
      free_year(segment_tree->years+iyear);
   }
   free(segment_tree->years);
   segment_tree->years = NULL;
   segment_tree->nyears = 0;
}

#ifdef _DEBUG
void print_segment(FILE *io_handle, trac_segment_t segment) {
   fprintf(io_handle, "         %d: ", segment.taskidx);
   print_timestamp_date(io_handle, segment.starttime);
   fprintf(io_handle, " -- ");
   print_timestamp_date(io_handle, segment.endtime);
   fprintf(io_handle, "\n");
}

void print_day(FILE *io_handle, trac_day_t day) {
   char *day_str = "unknown";
   if (day.nsegments > 0) {
      day_str = get_timestamp_day_str(day.segments[0].starttime);
   }
   fprintf(io_handle, "      (%02d) %s\n", day.day_of_month, day_str);
   for (int isegment=0; isegment<day.nsegments; isegment++) {
      print_segment(io_handle, day.segments[isegment]);
   }
}

void print_month(FILE *io_handle, trac_month_t month) {
   fprintf(io_handle, "   (%02d) %s\n", month.month_of_year, get_timestamp_month_str(month.month_of_year));
   for (int iday=0; iday<month.ndays; iday++) {
      print_day(io_handle, month.days[iday]);
   }
}

void print_year(FILE *io_handle, trac_year_t year) {
   fprintf(io_handle, "%04d:\n", year.year_number);
   for (int imonth=0; imonth<year.nmonths; imonth++) {
      print_month(io_handle, year.months[imonth]);
   }
}

void print_segment_tree(FILE *io_handle, trac_segment_tree_t segment_tree) {
   fprintf(io_handle, "+--------------+\n");
   fprintf(io_handle, "| Segment Tree |\n");
   fprintf(io_handle, "+--------------+\n");
   for (int iyear=0; iyear<segment_tree.nyears; iyear++) {
      print_year(io_handle, segment_tree.years[iyear]);
   }
}
#endif
