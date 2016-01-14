
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
#include "time-tracker_view.h"
#include "time-tracker_data.h"

#define STATUS_BAR_HEIGHT 16

static Window *s_main_window;

static const int16_t MARGIN = 8;
static const int16_t ICON_DIMENSIONS = 48;


////////////////////
// update procs for our three custom layers

static void bg_update_proc(Layer *layer, GContext *ctx) {
  TTrackerAppData *data = window_get_user_data(s_main_window);
  TTrackerAppMainWindowViewModel *model = &data->view_model;
  const GRect bounds = layer_get_bounds(layer);

  graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(model->bg_color.bottom, GColorWhite));

  GRect rect_top = bounds;
  graphics_fill_rect(ctx, rect_top, 0, GCornerNone);

//   graphics_context_set_fill_color(ctx, PBL_IF_COLOR_ELSE(model->bg_color.top, GColorWhite));
//   GRect rect_bottom = bounds;
//   rect_bottom.origin.y += y;
//   graphics_fill_rect(ctx, rect_bottom, 0, GCornerNone);
}

static void horizontal_ruler_update_proc(Layer *layer, GContext *ctx) {
  const GRect bounds = layer_get_bounds(layer);
  // y relative to layer's bounds to support clipping after some vertical scrolling
  const int16_t yy = 11;

  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));
  graphics_draw_line(ctx, GPoint(0, yy), GPoint(bounds.size.w, yy));
}

static void icon_layer_update_proc(Layer *layer, GContext *ctx) {
  TTrackerAppData *data = window_get_user_data(s_main_window);
  TTrackerAppMainWindowViewModel *model = &data->view_model;
  GDrawCommandImage *original_icon = model->icon.draw_command;
  if (!original_icon) {
    return;
  }

  graphics_context_set_antialiased(ctx, true);
  gdraw_command_image_draw(ctx, original_icon, GPoint(0, 0));
}

////////////////////
// App boilerplate

//! helper to construct the various text layers as they appear in this app
static GRect init_text_layer(Layer *parent_layer, TextLayer **text_layer, int16_t y, int16_t h, int16_t additional_right_margin, char *font_key) {
  // why "-1" (and then "+2")? because for this font we need to compensate for weird white-spacing
  const int16_t font_compensator = strcmp(font_key, FONT_KEY_LECO_38_BOLD_NUMBERS) == 0 ? 3 : 1;

  const GRect frame = GRect(MARGIN - font_compensator, y, layer_get_bounds(parent_layer).size.w - 2 * MARGIN + 2 * font_compensator - additional_right_margin, h);

  *text_layer = text_layer_create(frame);
  text_layer_set_background_color(*text_layer, GColorClear);
  text_layer_set_text_color(*text_layer, PBL_IF_COLOR_ELSE(GColorWhite, GColorBlack));
  text_layer_set_font(*text_layer, fonts_get_system_font(font_key));
  layer_add_child(parent_layer, text_layer_get_layer(*text_layer));
  return frame;
}

void init_statusbar_text_layer(Layer *parent, TextLayer **layer) {
  init_text_layer(parent, layer, 0, STATUS_BAR_HEIGHT, 0, FONT_KEY_GOTHIC_14);
  GRect sb_bounds = layer_get_bounds(text_layer_get_layer(*layer));
  sb_bounds.origin.y -= 1;
  layer_set_bounds(text_layer_get_layer(*layer), sb_bounds);
  text_layer_set_text_alignment(*layer, GTextAlignmentCenter);
}

//! sets the new data model
static void set_data_point(TTrackerAppData *data, TTrackerAppDataPoint *dp) {
  data->data_point = dp;
  ttracker_app_view_model_fill_all(&data->view_model, dp);
}

static void view_model_changed(struct TTrackerAppMainWindowViewModel *arg) {
  TTrackerAppMainWindowViewModel *model = (TTrackerAppMainWindowViewModel *)arg;

  TTrackerAppData *data = window_get_user_data(s_main_window);

  text_layer_set_text(data->day_of_week_layer, model->day_of_week);
  text_layer_set_text(data->work_time_layer, model->work_hours.text);
  text_layer_set_text(data->start_stop_time_layer, model->work_time.text);
  text_layer_set_text(data->description_layer, model->description);
  text_layer_set_text(data->pagination_layer, model->pagination.text);

  // make sure to redraw (if no string pointer changed none of the layers would be dirty)
  layer_mark_dirty(window_get_root_layer(s_main_window));
}

static void main_window_load(Window *window) {
  TTrackerAppData *data = window_get_user_data(window);
  data->view_model.announce_changed = view_model_changed;

  Layer *window_layer = window_get_root_layer(window);
  const GRect bounds = layer_get_bounds(window_layer);
  layer_set_update_proc(window_layer, bg_update_proc);

  data->horizontal_ruler_layer = layer_create(GRect(MARGIN, 40, bounds.size.w - 2 * MARGIN, 20));
  layer_set_update_proc(data->horizontal_ruler_layer, horizontal_ruler_update_proc);
  layer_add_child(window_layer, data->horizontal_ruler_layer);

  const int16_t narrow_buffer = 5; // current whitespacing would trim 3-digit temperature otherwise
  const int16_t narrow = ICON_DIMENSIONS + 2 - narrow_buffer;
  init_text_layer(window_layer, &data->day_of_week_layer, 23, 30, 0, FONT_KEY_GOTHIC_18_BOLD);
  const int16_t temperature_top = 49;
  init_text_layer(window_layer, &data->work_time_layer, temperature_top, 40, narrow, FONT_KEY_LECO_38_BOLD_NUMBERS);
  init_text_layer(window_layer, &data->start_stop_time_layer, 91, 19, narrow, FONT_KEY_GOTHIC_14);
  const int16_t description_top = 108;
  const int16_t description_height = bounds.size.h - description_top;
  init_text_layer(window_layer, &data->description_layer, description_top, description_height, 0, FONT_KEY_GOTHIC_24_BOLD);

  GRect icon_rect = GRect(0, 0, ICON_DIMENSIONS, ICON_DIMENSIONS);
  GRect alignment_rect = GRect(0, temperature_top + 10, bounds.size.w - MARGIN, 10);
  grect_align(&icon_rect, &alignment_rect, GAlignTopRight, false);
  data->icon_layer = layer_create(icon_rect);
  layer_set_update_proc(data->icon_layer, icon_layer_update_proc);
  layer_add_child(window_layer, data->icon_layer);

  init_statusbar_text_layer(window_layer, &data->fake_statusbar);
  text_layer_set_text(data->fake_statusbar, "9:41 AM");

  init_statusbar_text_layer(window_layer, &data->pagination_layer);
  text_layer_set_text_alignment(data->pagination_layer, GTextAlignmentRight);

  // propagate all view model content to the UI
  ttracker_app_main_window_view_model_announce_changed(&data->view_model);
}

static void main_window_unload(Window *window) {
  TTrackerAppData *data = window_get_user_data(window);
  data->view_model.announce_changed = NULL;
  ttracker_app_view_model_deinit(&data->view_model);

  layer_destroy(data->horizontal_ruler_layer);
  text_layer_destroy(data->day_of_week_layer);
  text_layer_destroy(data->work_time_layer);
  text_layer_destroy(data->start_stop_time_layer;
  text_layer_destroy(data->description_layer);
  layer_destroy(data->icon_layer);
  text_layer_destroy(data->fake_statusbar);
  text_layer_destroy(data->pagination_layer);
}


typedef enum {
  ScrollDirectionDown,
  ScrollDirectionUp,
} ScrollDirection;

static void ask_for_scroll(TTrackerAppData *data, ScrollDirection direction)
{
    TTrackerAppData *user_data = window_get_user_data(s_main_window);

    int delta = direction == ScrollDirectionUp ? -1 : +1;
    TTrackerAppDataPoint *next_data_point = ttracker_app_data_point_delta(data->data_point, delta);

    Animation *scroll_animation;

    if (next_data_point)
    {
        set_data_point(data, next_data_point);
    }

}


static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  TTrackerAppData *data = context;
  ask_for_scroll(data, ScrollDirectionUp);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  TTrackerAppData *data = context;
  ask_for_scroll(data, ScrollDirectionDown);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void init() {
  TTrackerAppData *data = malloc(sizeof(TTrackerAppData));
  memset(data, 0, sizeof(TTrackerAppData));

  TTrackerAppData *dp = TTrackerAppData(0);
  set_data_point(data, dp);

  s_main_window = window_create();
  window_set_click_config_provider_with_context(s_main_window, click_config_provider, data);
  window_set_user_data(s_main_window, data);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}


int main(void) {
  init();
  app_event_loop();
  deinit();
}
