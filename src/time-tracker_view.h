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
#include "time-tracker_data.h"

typedef struct {
  TTrackerAppDataPoint *data_point;
  TTrackerAppMainWindowViewModel view_model;
  TextLayer *fake_statusbar;
  TextLayer *pagination_layer;
  TextLayer *day_of_week_layer;
  Layer *horizontal_ruler_layer;
  TextLayer *work_time_layer;
  TextLayer *start_stop_time_layer;
  TextLayer *description_layer;
  Layer *icon_layer;
} TTrackerAppData;
