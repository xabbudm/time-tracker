#include <pebble.h>
#include "time_selection_menu.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_18_bold;
static MenuLayer *s_menulayer;
static TextLayer *s_heading_layer;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
  #endif
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_menulayer
  s_menulayer = menu_layer_create(GRect(5, 30, 130, 115));
  menu_layer_set_click_config_onto_window(s_menulayer, s_window);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_menulayer);
  
  // s_heading_layer
  s_heading_layer = text_layer_create(GRect(10, 5, 125, 24));
  text_layer_set_text(s_heading_layer, "Erfasse deine Zeit");
  text_layer_set_text_alignment(s_heading_layer, GTextAlignmentCenter);
  text_layer_set_font(s_heading_layer, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_heading_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  menu_layer_destroy(s_menulayer);
  text_layer_destroy(s_heading_layer);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_time_selection_menu(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_time_selection_menu(void) {
  window_stack_remove(s_window, true);
}
