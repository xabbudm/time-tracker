#include <pebble.h>
#include "time_window.h"
#include "../layers/selection_layer.h"

static char* selection_handle_get_text(int index, void *context) 
{
    TimeWindow *time_window = (TimeWindow*)context;
    snprintf(
        time_window->field_buffs[index],
        sizeof(time_window->field_buffs[0]), "%02d",
        (int)time_window->time.digits[index]
    );
    APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", time_window->field_buffs[index]);
    
    return time_window->field_buffs[index];
}

static void selection_handle_complete(void *context) {
  TimeWindow *time_window = (TimeWindow*)context;
  time_window->callbacks.time_complete(time_window->time, time_window);
}

static void selection_handle_inc(int index, uint8_t clicks, void *context) {
  TimeWindow *time_window = (TimeWindow*)context;
  time_window->time.digits[index]++;
  int threshold = 0;
  switch(index)
  {
    case 0 :
        threshold = TIME_WINDOW_HOURS_MAX_VALUE;
        break;
    case 1:
        threshold = TIME_WINDOW_MINUTES_MAX_VALUE;
      break;
  }
  if(time_window->time.digits[index] > threshold)
  {
      time_window->time.digits[index] = 0;
  }
}

static void selection_handle_dec(int index, uint8_t clicks, void *context)
{
    TimeWindow *time_window = (TimeWindow*)context;
    time_window->time.digits[index]--;
    int maxValue = 0;

    switch(index)
    {
    case 0 :
        maxValue = TIME_WINDOW_HOURS_MAX_VALUE;
        break;
    case 1:
        maxValue = TIME_WINDOW_MINUTES_MAX_VALUE;
        break;
    }


    if(time_window->time.digits[index] < 0)
    {
        time_window->time.digits[index] = maxValue;
    }
}

TimeWindow* time_window_create(TimeWindowCallbacks callbacks) {
  TimeWindow *time_window = (TimeWindow*)malloc(sizeof(TimeWindow));
  if (time_window) {
    time_window->window = window_create();
    time_window->callbacks = callbacks;
    if (time_window->window) {
      time_window->field_selection = 0;
      for(int i = 0; i < TIME_WINDOW_NUM_CELLS; i++) {
        time_window->time.digits[i] = 0;
      }

      // Get window parameters
      Layer *window_layer = window_get_root_layer(time_window->window);
      GRect bounds = layer_get_bounds(window_layer);

      // Main TextLayer
      const GEdgeInsets main_text_insets = {.top = 30};
      time_window->main_text = text_layer_create(grect_inset(bounds, main_text_insets));
      text_layer_set_text(time_window->main_text, "Time Required");
      text_layer_set_font(time_window->main_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
      text_layer_set_text_alignment(time_window->main_text, GTextAlignmentCenter);
      layer_add_child(window_layer, text_layer_get_layer(time_window->main_text));

      // Sub TextLayer
      const GEdgeInsets sub_text_insets = {.top = 115, .right = 5, .bottom = 10, .left = 5};
      time_window->sub_text = text_layer_create(grect_inset(bounds, sub_text_insets));
      text_layer_set_text(time_window->sub_text, "up/down to change time");
      text_layer_set_text_alignment(time_window->sub_text, GTextAlignmentCenter);
      text_layer_set_font(time_window->sub_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
      layer_add_child(window_layer, text_layer_get_layer(time_window->sub_text));

      // Create selection layer
      const GEdgeInsets selection_insets = GEdgeInsets(
        (bounds.size.h - TIME_WINDOW_SIZE.h) / 2,
        (bounds.size.w - TIME_WINDOW_SIZE.w) / 2);
      time_window->selection = selection_layer_create(grect_inset(bounds, selection_insets), TIME_WINDOW_NUM_CELLS);
      for (int i = 0; i < TIME_WINDOW_NUM_CELLS; i++) {
        selection_layer_set_cell_width(time_window->selection, i, 60);
      }
      selection_layer_set_cell_padding(time_window->selection, 6);
      selection_layer_set_active_bg_color(time_window->selection, GColorRed);
      selection_layer_set_inactive_bg_color(time_window->selection, GColorDarkGray);
      selection_layer_set_click_config_onto_window(time_window->selection, time_window->window);
      selection_layer_set_callbacks(time_window->selection, time_window, (SelectionLayerCallbacks) {
        .get_cell_text = selection_handle_get_text,
        .complete = selection_handle_complete,
        .increment = selection_handle_inc,
        .decrement = selection_handle_dec,
      });
      layer_add_child(window_get_root_layer(time_window->window), time_window->selection);

      // Create status bar
      time_window->status = status_bar_layer_create();
      status_bar_layer_set_colors(time_window->status, GColorClear, GColorBlack);
      layer_add_child(window_layer, status_bar_layer_get_layer(time_window->status));
      return time_window;
    }
  }

  APP_LOG(APP_LOG_LEVEL_ERROR, "Failed to create TimeWindow");
  return NULL;
}

void time_window_destroy(TimeWindow *time_window) {
  if (time_window) {
    status_bar_layer_destroy(time_window->status);
    selection_layer_destroy(time_window->selection);
    text_layer_destroy(time_window->sub_text);
    text_layer_destroy(time_window->main_text);
    free(time_window);
    time_window = NULL;
    return;
  }
}

void time_window_push(TimeWindow *time_window, bool animated) {
  window_stack_push(time_window->window, animated);
}

void time_window_pop(TimeWindow *time_window, bool animated) {
  window_stack_remove(time_window->window, animated);
}

bool time_window_get_topmost_window(TimeWindow *time_window) {
  return window_stack_get_top_window() == time_window->window;
}

void tims_window_set_highlight_color(TimeWindow *time_window, GColor color) {
  time_window->highlight_color = color;
  selection_layer_set_active_bg_color(time_window->selection, color);
}
