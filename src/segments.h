#ifndef SEGMENTS_H
#define SEGMENTS_H

#include "tasks.h"
#include "timestamps.h"

#ifdef _DEBUG
#include <stdio.h>
#endif

typedef struct trac_segment_type trac_segment_t;
struct trac_segment_type {
   int taskidx;
   trac_timestamp_t starttime;
   trac_timestamp_t endtime;
   int time;
};

typedef struct trac_day_type trac_day_t;
struct trac_day_type {
   int day_of_month;
   int nsegments;
   trac_segment_t *segments;
};

typedef struct trac_month_type trac_month_t;
struct trac_month_type {
   int month_of_year;
   int ndays;
   trac_day_t *days;
};

typedef struct trac_year_type trac_year_t;
struct trac_year_type {
   int year_number;
   int nmonths;
   trac_month_t *months;
};

typedef struct trac_segment_tree_type trac_segment_tree_t;
struct trac_segment_tree_type {
   int nyears;
   trac_year_t *years;
};

trac_segment_tree_t new_segment_tree();
trac_year_t *new_year(trac_segment_tree_t *segment_tree, int year_number);
trac_month_t *new_month(trac_year_t *year, int month_of_year);
trac_day_t *new_day(trac_month_t *month, int day_of_month);
trac_segment_t *new_segment(trac_segment_tree_t *segment_tree, int taskidx, trac_timestamp_t starttime);
trac_segment_t *new_segment_current_timestamp(trac_segment_tree_t *segment_tree, int taskidx);

int get_year_idx_from_number(trac_segment_tree_t *segment_tree, int year_number);
int get_month_idx_from_number(trac_year_t *year, int month_of_year);
int get_day_idx_from_number(trac_month_t *month, int day_of_month);

void remove_segment(trac_segment_tree_t *segment_tree, int year_number, int month_of_year, int day_of_month, int segment_idx);
void remove_day(trac_segment_tree_t *segment_tree, int year_number, int month_of_year, int day_of_month);
void remove_month(trac_segment_tree_t *segment_tree, int year_number, int month_of_year);
void remove_year(trac_segment_tree_t *segment_tree, int year_number);

void free_segment(trac_segment_t *segment);
void free_day(trac_day_t *day);
void free_month(trac_month_t *month);
void free_year(trac_year_t *year);
void free_segment_tree(trac_segment_tree_t *segment_tree);

#ifdef _DEBUG
void print_segment(FILE *io_handle, trac_segment_t segments);
void print_day(FILE *io_handle, trac_day_t day);
void print_month(FILE *io_handle, trac_month_t month);
void print_year(FILE *io_handle, trac_year_t year);
void print_segment_tree(FILE *io_handle, trac_segment_tree_t segment_tree);
#endif

#endif
