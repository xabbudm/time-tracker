/*
 * Copyright 2016 Daniel Muhle (dmuhle at gmail dot com)
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

typedef enum SelectState
{
    START,
    STOP,
    PAUSE
} SelectState;

typedef struct 
{
    SelectState select_state;
    time_t logged_time;
} SelectedTime;

typedef void (*TimeSelectionWindowComplete)(SelectedTime selected_time, void *context);

typedef struct TimeSelectionWindowCallbacks {
    TimeSelectionWindowComplete time_selection_complete;
} TimeSelectionWindowCallbacks;


static Window *s_window;
static GFont s_res_gothic_18_bold;
static MenuLayer *s_menu_layer;
static TextLayer *s_heading_layer;

typedef struct 
{
    TextLayer *heading_layer;
    MenuLayer *menu_layer;
    TimeSelectionWindowCallbacks callbacks;

    SelectedTime selected_time;
} TimeSelectionWindow;


void show_time_selection_menu(void);
void hide_time_selection_menu(void);

/*
 * Creates a new Timeindow in memory but does not push it into view
 *  time_window_callbacks: callbacks for communication
 *  returns: a pointer to a new TimeWindow structure
 */
TimeSelectionWindow* time_selection_menu_create(TimeSelectionWindowCallbacks callbacks);

/*
 * Destroys an existing TimeWindow
 *  time_window: a pointer to the TimeWindow being destroyed
 */
void time_selection_menu_destroy(TimeSelectionWindow *time_selection_window);

/*
 * Push the window onto the stack
 *  time_window: a pointer to the TimeWindow being pushed
 *  animated: whether to animate the push or not
 */
void time_selection_menu_push(TimeSelectionWindow *time_selection_window, bool animated);

/*
 * Pop the window off the stack
 *  time_window: a pointer to the TimeWindow to pop
 *  animated: whether to animate the pop or not
 */
void time_selection_menu_pop(TimeSelectionWindow *time_selection_window, bool animated);

