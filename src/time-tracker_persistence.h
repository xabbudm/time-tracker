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

// add versioning for future updates
#define TIME_PERSISTENCE_VERSION_KEY 64
#define TIME_PERSISTENCE_VERSION 1

#define TIME_PERSISTENCE_CURRENT_YEAR_KEY 0
#define TIME_PERSISTENCE_CURRENT_MONTH_KEY 1
#define TIME_PERSISTENCE_CURRENT_WEEK_KEY 2
#define TIME_PERSISTENCE_CURRENT_DAY_KEY 3

#define TIME_PERSISTENCE_JAN_SUM_KEY  4
#define TIME_PERSISTENCE_FEB_SUM_KEY  5
#define TIME_PERSISTENCE_MAR_SUM_KEY  6
#define TIME_PERSISTENCE_APR_SUM_KEY  7
#define TIME_PERSISTENCE_MAY_SUM_KEY  8
#define TIME_PERSISTENCE_JUN_SUM_KEY  9
#define TIME_PERSISTENCE_JUL_SUM_KEY 10
#define TIME_PERSISTENCE_AUG_SUM_KEY 11
#define TIME_PERSISTENCE_SEP_SUM_KEY 12
#define TIME_PERSISTENCE_OCT_SUM_KEY 13
#define TIME_PERSISTENCE_NOV_SUM_KEY 14
#define TIME_PERSISTENCE_DEC_SUM_KEY 15

#define TIME_PERSISTENCE_START_TIME_KEY 16
#define TIME_PERSISTENCE_VALID_START_TIME_KEY 17

#define TIME_PERSISTENCE_END_TIME_KEY 18
#define TIME_PERSISTENCE_VALID_END_TIME_KEY 19

#define TIME_PERSISTENCE_PAUSE_TIME_KEY 18
#define TIME_PERSISTENCE_VALID_PAUSE_TIME_KEY 19

#define 
