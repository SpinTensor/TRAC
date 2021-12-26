#include <stdlib.h>
#include <time.h>

#include "timestamps.h"

#ifdef _DEBUG
#include <stdio.h>
#endif

trac_timestamp_t get_current_timestamp() {
   trac_timestamp_t timestamp;
   timestamp.seconds_since_epoch = time(NULL);
   timestamp.date = *localtime(&timestamp.seconds_since_epoch);
   timestamp.date.tm_year += 1900;
   timestamp.date.tm_mon++;
   return timestamp;
}

trac_timestamp_t set_timestamp_seconds_since_epoch(unsigned long long s_since_epoch) {
   trac_timestamp_t timestamp;
   timestamp.seconds_since_epoch = s_since_epoch;
   timestamp.date = *localtime(&timestamp.seconds_since_epoch);
   timestamp.date.tm_year += 1900;
   timestamp.date.tm_mon++;
   return timestamp;

}

trac_timestamp_t set_timestamp_date(int year, int month, int day, int hour, int minute, int second) {
   trac_timestamp_t timestamp;
   timestamp.date.tm_year = year-1900;
   timestamp.date.tm_mon = month-1;
   timestamp.date.tm_mday = day;
   timestamp.date.tm_hour = hour;
   timestamp.date.tm_min = minute;
   timestamp.date.tm_sec = second;
   timestamp.date.tm_isdst = -1;

   timestamp.seconds_since_epoch = mktime(&timestamp.date);
   timestamp.date.tm_year += 1900;
   timestamp.date.tm_mon++;
   return timestamp;
}

unsigned long long get_timestamp_seconds_since_epoch(trac_timestamp_t timestamp) {
   return (unsigned long long) timestamp.seconds_since_epoch;
}

int get_timestamp_year(trac_timestamp_t timestamp) {
   return (int) timestamp.date.tm_year;
}

int get_timestamp_month(trac_timestamp_t timestamp) {
   return (int) timestamp.date.tm_mon;
}

char *get_timestamp_month_str(int month_of_year) {
   char *month_strs[] = {"January",
                         "February",
                         "March",
                         "April",
                         "May",
                         "June",
                         "July",
                         "August",
                         "September",
                         "October",
                         "November",
                         "December"};
   return month_strs[month_of_year-1];
}

int get_timestamp_day(trac_timestamp_t timestamp) {
   return (int) timestamp.date.tm_mday;
}

int get_timestamp_hour(trac_timestamp_t timestamp) {
   return (int) timestamp.date.tm_hour;
}

int get_timestamp_minute(trac_timestamp_t timestamp) {
   return (int) timestamp.date.tm_min;
}

int get_timestamp_second(trac_timestamp_t timestamp) {
   return (int) timestamp.date.tm_sec;
}

#ifdef _DEBUG
void print_timestamp_seconds_since_epoch(FILE *io_handle, trac_timestamp_t timestamp) {
   fprintf(io_handle, "%lld", get_timestamp_seconds_since_epoch(timestamp));
}

void print_timestamp_date(FILE *io_handle, trac_timestamp_t timestamp) {
   fprintf(io_handle, "%02d:%02d:%02d %02d.%02d.%04d",
           get_timestamp_hour(timestamp),
           get_timestamp_minute(timestamp),
           get_timestamp_second(timestamp),
           get_timestamp_day(timestamp),
           get_timestamp_month(timestamp),
           get_timestamp_year(timestamp));
}
#endif
