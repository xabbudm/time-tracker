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


#include <pebble.h>
#include "time-tracker_data.h"
#include "time-tracker_resources.h"

void ttracker_app_main_window_view_model_announce_changed(TTrackerAppMainWindowViewModel *model)
{
    if (model->announce_changed)
    {
        model->announce_changed((struct TTrackerAppMainWindowViewModel *) model);
    }
}

void ttracker_app_view_model_set_work_times(TTrackerAppMainWindowViewModel *model, time_t start, time_t stop, uint16_t pause)
{
    model->work_time.start = start;
    model->work_time.stop = stop;
    model->work_time.pause = pause;

    struct tm* my_time = localtime(&start);
    int start_hour = my_time->tm_hour;
    int start_min = my_time->tm_min;
    
    my_time = localtime(&stop);
    int end_hour = my_time->tm_hour;
    int end_min = my_time->tm_min;
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%d:%d - %d:%d (%d)", start_hour, start_min, end_hour, end_min, pause/60);
    
    snprintf(
         model->work_time.text,
         sizeof(model->work_time.text),
         "%02d:%02d - %02d:%02d\n(P %4d m)",
         start_hour,
         start_min,
         end_hour,
         end_min,
         pause/60);
}

void ttracker_app_view_model_set_work_hours(TTrackerAppMainWindowViewModel *model, int32_t hours)
{
    const int SEC_PER_HOUR = 3600;
    const int SEC_PER_MINUTE = 60;
    model->work_hours.hours = hours;

    int hour_part = hours / SEC_PER_HOUR;
    int hour_diff = hours - hour_part*SEC_PER_HOUR;

    int minute_part = hour_diff / SECONDS_PER_MINUTE;
    snprintf(
        model->work_hours.text,
        sizeof(model->work_hours.text),
        "%02d:%02d",
        hour_part,
        minute_part);
}

void ttracker_app_view_model_set_icon(TTrackerAppMainWindowViewModel *model, GBitmap *image)
{
    if (0 != image)
    {
        if (0 != model->icon.draw_command)
        {
            gbitmap_destroy(model->icon.draw_command);
        }
        else
        {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Bitmap was already 0");          
        }
        
        model->icon.draw_command = image;
        ttracker_app_main_window_view_model_announce_changed(model);
    }
    else
    {
        model->icon.draw_command = 0;
    }
}

TTrackerDataViewNumbers ttracker_app_data_point_view_model_times(TTrackerAppDataPoint *data_point) 
{

  int time_diff_seconds = data_point->stop - data_point->start - data_point->pause;

  return (TTrackerDataViewNumbers){
      .start = data_point->start,
      .stop  = data_point->stop,
      .pause = data_point->pause,
      .total_hours = time_diff_seconds
  };
}

int ttracker_app_index_of_data_point(TTrackerAppDataPoint *dp);

void ttracker_app_view_model_fill_strings_and_pagination(TTrackerAppMainWindowViewModel *view_model, TTrackerAppDataPoint *data_point) {
  view_model->day_of_week = data_point->day_of_week;
  view_model->description = data_point->description;

  view_model->pagination.idx = (int16_t)(1 + ttracker_app_index_of_data_point(data_point));
  view_model->pagination.num = (int16_t)ttracker_app_num_data_points();
  snprintf(view_model->pagination.text, sizeof(view_model->pagination.text), "%02d/%02d", view_model->pagination.idx, view_model->pagination.num);
  ttracker_app_main_window_view_model_announce_changed(view_model);
}




void ttracker_view_model_fill_numbers(TTrackerAppMainWindowViewModel *model, TTrackerDataViewNumbers numbers)
{
  ttracker_app_view_model_set_work_hours(model, numbers.total_hours);
  ttracker_app_view_model_set_work_times(model, numbers.start, numbers.stop, numbers.pause);
}

void ttracker_app_view_model_fill_colors(TTrackerAppMainWindowViewModel *model, GColor color)
{
  model->bg_color.top = color;
  model->bg_color.bottom = color;
  ttracker_app_main_window_view_model_announce_changed(model);
}

GColor  ttracker_app_data_point_color(TTrackerAppDataPoint *data_point)
{
    // make color dependend on total work time
    // return data_point->current > 90 ? GColorOrange : GColorPictonBlue;
    return GColorPictonBlue;
}

void ttracker_app_view_model_fill_all(TTrackerAppMainWindowViewModel *model, TTrackerAppDataPoint *data_point)
{
    TTrackerAppMainWindowViewModelFunc annouce_changed = model->announce_changed;
    GBitmap* image = model->icon.draw_command;
    
    memset(model, 0, sizeof(*model));
    gbitmap_destroy(image);
    
    model->announce_changed = annouce_changed;
    ttracker_app_view_model_fill_strings_and_pagination(model, data_point);
    ttracker_app_view_model_set_icon(model, ttracker_app_resources_get_icon());
    ttracker_app_view_model_fill_colors(model, ttracker_app_data_point_color(data_point));
    ttracker_view_model_fill_numbers(model, ttracker_app_data_point_view_model_times(data_point));

    ttracker_app_main_window_view_model_announce_changed(model);
}

void ttracker_app_view_model_deinit(TTrackerAppMainWindowViewModel *model) 
{
    GBitmap* image = model->icon.draw_command;
    
    ttracker_app_view_model_set_icon(model, NULL);

    gbitmap_destroy(image);      


}

static TTrackerAppDataPoint s_data_points[] = {
    {
        .day_of_week = "Montag",
        .description = ":-(",
        .i_day_of_week = 0,
        .start = 1452664800,
        .stop = 1452698100,
        .pause = 45*60,
    },
    {
        .day_of_week = "Dienstag",
        .description = ":-I",
        .i_day_of_week = 1,
        .start = 1452751200,
        .stop = 1452784500,
        .pause = 45*60,
    },
    {
        .day_of_week = "Mittwoch",
        .description = ":-)",
        .i_day_of_week = 2,
        .start = 1452837600,
        .stop = 1452870900,
        .pause = 45*60,
    },
    {
        .day_of_week = "Donnerstag",
        .description = ":-()",
        .i_day_of_week = 3,
        .start = 1452924000,
        .stop = 1452957300,
        .pause = 45*60,
    },
    {
        .day_of_week = "Freitag",
        .description = ":-()",
        .i_day_of_week = 4,
        .start = 1453010400,
        .stop = 1453032900,
        .pause = 15*60,
    },
};

int ttracker_app_num_data_points(void) {
  return ARRAY_LENGTH(s_data_points);
}

TTrackerAppDataPoint *ttracker_app_data_point_at(int idx) {
  if (idx < 0 || idx > ttracker_app_num_data_points() - 1) {
    return NULL;
  }

  return &s_data_points[idx];
}

int ttracker_app_index_of_data_point(TTrackerAppDataPoint *dp) {
  for (int i = 0; i < ttracker_app_num_data_points(); i++) {
    if (dp == ttracker_app_data_point_at(i)) {
      return i;
    }
  }
  return -1;
}

TTrackerAppDataPoint *ttracker_app_data_point_delta(TTrackerAppDataPoint *dp, int delta)
{
    int idx = ttracker_app_index_of_data_point(dp);
    if (idx < 0)
    {
        return NULL;
    }
    return ttracker_app_data_point_at(idx + delta);
}




