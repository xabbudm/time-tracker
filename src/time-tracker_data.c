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

static const uint32_t SECONDS_PER_HOUR   = 3600;
static const uint32_t SECONDS_PER_MINUTE =   60;

void ttracker_app_main_window_view_model_announce_changed(TTrackerAppMainWindowViewModel *model) 
{
    if (model->announce_changed)     
    {
        model->announce_changed((struct TTrackerAppMainWindowViewModel *) model);
    }
}
    
void ttracker_app_view_model_set_work_times(TTrackerAppMainWindowViewModel *model, time_t start, time_t stop, int16_t pause) 
{
    model->worktime.start = start;
    model->worktime.stop = stop;
    model->worktime.pause = pause;
    
    struct tm* start_time = localtime(&start);
    struct tm* end_time = localtime(&stop);
    
     //snprintf(model->highlow.text, sizeof(model->highlow.text), "HI %d°, LO %d°", model->highlow.high, model->highlow.low);
}

void ttracker_app_view_model_set_work_hours(TTrackerAppMainWindowViewModel *model, int32_t hours) 
{
    model->work_hours.hours = hours;
  //snprintf(model->temperature.text, sizeof(model->temperature.text), "%d°", model->temperature.value);
}

void ttracker_app_view_model_set_icon(TTrackerAppMainWindowViewModel *model, GDrawCommandImage *image) 
{
    free(model->icon.draw_command);
    model->icon.draw_command = image;
    ttracker_app_main_window_view_model_announce_changed(model);
}

TTrackerDataViewNumbers ttracker_app_data_point_view_model_times(TTrackerAppDataPoint *data_point) {
  return (TTrackerDataViewNumbers){
      .start = data_point->start,
      .stop  = data_point->stop,
      .pause = data_point->pause,
  };
}

int weather_app_index_of_data_point(WeatherAppDataPoint *dp);

void weather_app_view_model_fill_strings_and_pagination(WeatherAppMainWindowViewModel *view_model, WeatherAppDataPoint *data_point) {
  view_model->city = data_point->city;
  view_model->description = data_point->description;

  view_model->pagination.idx = (int16_t)(1 + weather_app_index_of_data_point(data_point));
  view_model->pagination.num = (int16_t)weather_app_num_data_points();
  snprintf(view_model->pagination.text, sizeof(view_model->pagination.text), "%d/%d", view_model->pagination.idx, view_model->pagination.num);
  weather_app_main_window_view_model_announce_changed(view_model);
}


GDrawCommandImage *weather_app_data_point_create_icon(WeatherAppDataPoint *data_point) {
  return weather_app_resources_get_icon(data_point->icon);
}


void weather_view_model_fill_numbers(WeatherAppMainWindowViewModel *model, WeatherDataViewNumbers numbers) {
  weather_app_view_model_set_temperature(model, numbers.temperature);
  weather_app_view_model_set_highlow(model, numbers.high, numbers.low);
}

void weather_app_view_model_fill_colors(WeatherAppMainWindowViewModel *model, GColor color) {
  model->bg_color.top = color;
  model->bg_color.bottom = color;
  weather_app_main_window_view_model_announce_changed(model);
}

GColor weather_app_data_point_color(WeatherAppDataPoint *data_point) {
  return data_point->current > 90 ? GColorOrange : GColorPictonBlue;
}

void weather_app_view_model_fill_all(WeatherAppMainWindowViewModel *model, WeatherAppDataPoint *data_point) {
  WeatherAppMainWindowViewModelFunc annouce_changed = model->announce_changed;
  memset(model, 0, sizeof(*model));
  model->announce_changed = annouce_changed;
  weather_app_view_model_fill_strings_and_pagination(model, data_point);
  weather_app_view_model_set_icon(model, weather_app_data_point_create_icon(data_point));
  weather_app_view_model_fill_colors(model, weather_app_data_point_color(data_point));
  weather_view_model_fill_numbers(model, weather_app_data_point_view_model_numbers(data_point));

  weather_app_main_window_view_model_announce_changed(model);
}

void weather_app_view_model_deinit(WeatherAppMainWindowViewModel *model) {
  weather_app_view_model_set_icon(model, NULL);
}

static WeatherAppDataPoint s_data_points[] = {
    {
        .city = "PALO ALTO",
        .description = "Light Rain.",
        .icon = WEATHER_APP_ICON_LIGHT_RAIN,
        .current = 68,
        .high = 70,
        .low = 60,
    },
    {
        .city = "LOS ANGELES",
        .description = "Clear throughout the day.",
        .icon = WEATHER_APP_ICON_SUNNY_DAY,
        .current = 100,
        .high = 100,
        .low = 80,
    },
    {
        .city = "SAN FRANCISCO",
        .description = "Rain and Fog.",
        .icon = WEATHER_APP_ICON_HEAVY_SNOW,
        .current = 60,
        .high = 62,
        .low = 56,
    },
    {
        .city = "SAN DIEGO",
        .description = "Surfboard :)",
        .icon = WEATHER_APP_ICON_GENERIC_WEATHER,
        .current = 110,
        .high = 120,
        .low = 9,
    },
};

int weather_app_num_data_points(void) {
  return ARRAY_LENGTH(s_data_points);
}

WeatherAppDataPoint *weather_app_data_point_at(int idx) {
  if (idx < 0 || idx > weather_app_num_data_points() - 1) {
    return NULL;
  }

  return &s_data_points[idx];
}

int weather_app_index_of_data_point(WeatherAppDataPoint *dp) {
  for (int i = 0; i < weather_app_num_data_points(); i++) {
    if (dp == weather_app_data_point_at(i)) {
      return i;
    }
  }
  return -1;
}

WeatherAppDataPoint *weather_app_data_point_delta(WeatherAppDataPoint *dp, int delta) {
  int idx = weather_app_index_of_data_point(dp);
  if (idx < 0) {
    return NULL;
  }
  return weather_app_data_point_at(idx + delta);
}




