#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <time.h>

#ifdef _DEBUG
#include <stdio.h>
#endif

typedef struct trac_timestamp_type trac_timestamp_t;
// record the time since the epoch
struct trac_timestamp_type {
   time_t seconds_since_epoch;
   struct tm date;
};

trac_timestamp_t get_current_timestamp();
trac_timestamp_t set_timestamp_seconds_since_epoch(unsigned long long s_since_epoch);
trac_timestamp_t set_timestamp_date(int year, int month, int day, int hour, int minute, int second);

unsigned long long get_timestamp_seconds_since_epoch(trac_timestamp_t timestamp);
int get_timestamp_year(trac_timestamp_t timestamp);
int get_timestamp_month(trac_timestamp_t timestamp);
int get_timestamp_day(trac_timestamp_t timestamp);
int get_timestamp_hour(trac_timestamp_t timestamp);
int get_timestamp_minute(trac_timestamp_t timestamp);
int get_timestamp_second(trac_timestamp_t timestamp);

#ifdef _DEBUG
void print_timestamp_seconds_since_epoch(FILE *io_handle, trac_timestamp_t timestamp);
void print_timestamp_date(FILE *io_handle, trac_timestamp_t timestamp);
#endif

#endif
