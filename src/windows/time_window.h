#pragma once
// Pebble Time Window component adapted from https://github.com/pebble-examples/ui-patterns

#include <pebble.h>

#define TIME_WINDOW_NUM_CELLS 3
#define TIME_WINDOW_HOURS_MAX_VALUE 23
#define TIME_WINDOW_MINUTES_MAX_VALUE 59
#define TIME_WINDOW_SIZE GSize(128, 34)


typedef struct 
{
    int digits[TIME_WINDOW_NUM_CELLS];
} TIME;

typedef enum ETimeState
{
    LOG_STARTED,
    LOG_PAUSED,
    LOG_FINISHED
} ETimeState;


typedef void (*TimeWindowComplete)(TIME time, ETimeState state, void *context);

typedef struct TimeWindowCallbacks {
    TimeWindowComplete time_complete;
} TimeWindowCallbacks;



typedef struct {
  Window *window;
  TextLayer *main_text, *sub_text;
  Layer *selection;
  GColor highlight_color;
  StatusBarLayer *status;
  TimeWindowCallbacks callbacks;

  TIME time;
  ETimeState state;
  char field_buffs[TIME_WINDOW_NUM_CELLS][3];
  int8_t field_selection;
} TimeWindow;

/*
 * Creates a new Timeindow in memory but does not push it into view
 *  time_window_callbacks: callbacks for communication
 *  returns: a pointer to a new TimeWindow structure
 */
TimeWindow* time_window_create(TimeWindowCallbacks time_window_callbacks, ETimeState state);

/*
 * Destroys an existing TimeWindow
 *  time_window: a pointer to the TimeWindow being destroyed
 */
void time_window_destroy(TimeWindow *time_window);

/*
 * Push the window onto the stack
 *  time_window: a pointer to the TimeWindow being pushed
 *  animated: whether to animate the push or not
 */
void time_window_push(TimeWindow *time_window, bool animated);

/*
 * Pop the window off the stack
 *  time_window: a pointer to the TimeWindow to pop
 *  animated: whether to animate the pop or not
 */
void time_window_pop(TimeWindow *time_window, bool animated);

/*
 * Gets whether it is the topmost window or not
 *  time_window: a pointer to the TimeWindow being checked
 *  returns: a boolean indicating if it is the topmost window
 */
bool time_window_get_topmost_window(TimeWindow *time_window);

/*
 * Sets the over-all color scheme of the window
 *  color: the GColor to set the highlight to
 */
void time_window_set_highlight_color(TimeWindow *time_window, GColor color);
