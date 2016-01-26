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
    Window *window;
    TextLayer *heading_layer;
    MenuLayer *menu_layer;
    TimeSelectionWindowCallbacks callbacks;

    SelectedTime selected_time;
} TimeSelectionWindow;


void show_time_selection_menu(void);
void hide_time_selection_menu(void);
