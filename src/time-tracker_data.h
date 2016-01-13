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
    float  value;
    char text[8];
  } work_hours;
  struct {
    GDrawCommandImage *draw_command;
    int32_t to_square_normalized;
  } icon;
  struct {
    int16_t idx;
    int16_t num;
    char text[8];
  } pagination;
  struct {
    int16_t start;
    int16_t stop;
    int16_t pause;  
    char text[20];
  } work_time;
  char *description;
} TTrackerAppMainWindowViewModel;

//! calls model's .announce_changed or does nothing if NULL
void ttracker_app_main_window_view_model_announce_changed(TTrackerAppMainWindowViewModel *model);

typedef struct {
  char *day_of_week;
  int icon;
  int16_t start;
  int16_t stop;
  int16_t pause;
} TTrackerAppDataPoint;

typedef struct {
  int16_t start;
  int16_t stop;
  int16_t pause;
} TTrackerDataViewNumbers;

