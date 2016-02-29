/*
 * Copyright 2015 Daniel Muhle (dmuhle at gmail dot com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <pebble.h>

struct TTrackerAppMainWindowViewModel;

typedef void (*TTrackerAppMainWindowViewModelFunc)(struct TTrackerAppMainWindowViewModel* model);

typedef struct {
  TTrackerAppMainWindowViewModelFunc announce_changed;
  struct {
    GColor top;
    GColor bottom;
    int32_t to_bottom_normalized;
  } bg_color;
  char *day_of_week;
  struct {
    int32_t hours;
    char text[8];
  } work_hours;
  struct {
    GBitmap  *draw_command;
    int32_t to_square_normalized;
  } icon;
  struct {
    int16_t idx;
    int16_t num;
    char text[8];
  } pagination;
  struct {
    time_t start;
    time_t stop;
    uint16_t pause;
    char text[32];
  } work_time;
  char *description;
} TTrackerAppMainWindowViewModel;

//! calls model's .announce_changed or does nothing if NULL
void ttracker_app_main_window_view_model_announce_changed(TTrackerAppMainWindowViewModel *model);

typedef struct {
  char *day_of_week;
  int i_day_of_week;
  time_t start;
  time_t stop;
  uint16_t pause;
  char* description;
} TTrackerAppDataPoint;

typedef struct
{
    time_t start;
    time_t stop;
    uint16_t pause;
    uint16_t total_hours;

} TTrackerDataViewNumbers;

/* typedef enum ETrackingState */
/* { */
/*     START_TIME, */
/*     PAUSE_TIME, */
/*     STOP_TIME */
/* } ETrackingState; */

void ttracker_app_view_model_set_work_times(
					    TTrackerAppMainWindowViewModel *model, 
					    time_t start, time_t stop, uint16_t pause);

void ttracker_app_view_model_set_work_hours(TTrackerAppMainWindowViewModel *model, int32_t hours);
void ttracker_app_view_model_set_icon(TTrackerAppMainWindowViewModel *model, GBitmap *image);

TTrackerDataViewNumbers ttracker_app_data_point_view_model_times(TTrackerAppDataPoint *data_point);

void ttracker_app_view_model_fill_strings_and_pagination(
							 TTrackerAppMainWindowViewModel *view_model,
							 TTrackerAppDataPoint *data_point);

void ttracker_view_model_fill_numbers(TTrackerAppMainWindowViewModel *model, 
				      TTrackerDataViewNumbers numbers);

void ttracker_app_view_model_fill_all(TTrackerAppMainWindowViewModel *model, 
				      TTrackerAppDataPoint *data_point);

void ttracker_app_view_model_fill_colors(TTrackerAppMainWindowViewModel *model, GColor color);

void ttracker_app_view_model_deinit(TTrackerAppMainWindowViewModel *model);

GColor ttracker_app_data_point_color(TTrackerAppDataPoint *data_point);

int ttracker_app_num_data_points(void);

TTrackerAppDataPoint *ttracker_app_data_point_at(int idx);
TTrackerAppDataPoint *ttracker_app_data_point_delta(TTrackerAppDataPoint *dp, int delta);

