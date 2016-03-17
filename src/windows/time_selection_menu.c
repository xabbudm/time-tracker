#include <pebble.h>

#include "time_selection_menu.h"
#include "time_window.h"

#define NUM_MENU_ENTRIES 3
#define CHECKBOX_WINDOW_CELL_HEIGHT 44

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_18_bold;
static MenuLayer *s_menu_layer;
static TextLayer *s_heading_layer;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_MENU_ENTRIES;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "Startzeit", NULL, NULL);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "Pause ...", NULL, NULL);
      break;
    case 2:
      menu_cell_basic_draw(ctx, cell_layer, "Stopzeit", NULL, NULL);
      break;
    default:
      break;
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  return PBL_IF_ROUND_ELSE(
    menu_layer_is_index_selected(menu_layer, cell_index) ?
      MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT : MENU_CELL_ROUND_UNFOCUSED_TALL_CELL_HEIGHT,
    CHECKBOX_WINDOW_CELL_HEIGHT);
}


static void time_complete_callback(TIME time, ETimeState state, void *context) {

    switch(state)
    {
        case LOG_STARTED:
            APP_LOG(APP_LOG_LEVEL_INFO, "Logged Start Time was %d:%d", time.digits[0], time.digits[1]);
            break;
        case LOG_PAUSED:
            APP_LOG(APP_LOG_LEVEL_INFO, "Logged Pause Time was %d:%d", time.digits[0], time.digits[1]);
            break;
        case LOG_FINISHED:
            APP_LOG(APP_LOG_LEVEL_INFO, "Logged Stop Time was %d:%d", time.digits[0], time.digits[1]);
            break;
    }    
    
    time_window_pop((TimeWindow*)context, true);
    //time_window_destroy((TimeWindow*)context);
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  TimeWindow *time_window = 0;
  switch(cell_index->row) {
    case 0:
        {
        time_window = time_window_create((TimeWindowCallbacks) {
          .time_complete = time_complete_callback
        }, LOG_STARTED);
        time_window_push(time_window, true);
      }
      break;
    case 1:
        {
        time_window = time_window_create((TimeWindowCallbacks) {
          .time_complete = time_complete_callback
        }, LOG_PAUSED);
        time_window_push(time_window, true);
      }
      break;
    case 2:
        {
        time_window = time_window_create((TimeWindowCallbacks) {
          .time_complete = time_complete_callback
        }, LOG_FINISHED);
        time_window_push(time_window, true);
      }
      break;
    default:
      break;
  }
}

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
  #endif

  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_menulayer
  s_menu_layer = menu_layer_create(GRect(5, 30, 130, 115));
  menu_layer_set_click_config_onto_window(s_menu_layer, s_window);

  #if defined(PBL_COLOR)
  menu_layer_set_normal_colors(s_menu_layer, GColorBlack, GColorWhite);
  menu_layer_set_highlight_colors(s_menu_layer, GColorRed, GColorWhite);
#endif
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_callback,
  });

  layer_add_child(window_get_root_layer(s_window), (Layer *)s_menu_layer);

  // s_heading_layer
  s_heading_layer = text_layer_create(GRect(10, 5, 125, 24));
  text_layer_set_text(s_heading_layer, "Erfasse deine Zeit");
  text_layer_set_text_alignment(s_heading_layer, GTextAlignmentCenter);
  text_layer_set_font(s_heading_layer, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_heading_layer);
}

static void destroy_ui(void) 
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "TimeSelectionMenu unload() called");
    window_destroy(s_window);
    menu_layer_destroy(s_menu_layer);
    text_layer_destroy(s_heading_layer);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) 
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Selection Menu - window_unload called");
    destroy_ui();
}

void show_time_selection_menu(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_time_selection_menu(void) 
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Selection Menu - hide_selection_menu called");
    window_stack_remove(s_window, true);
}

/*
 * Creates a new Timeindow in memory but does not push it into view
 *  time_window_callbacks: callbacks for communication
 *  returns: a pointer to a new TimeWindow structure
 */
TimeSelectionWindow* time_selection_menu_create(TimeSelectionWindowCallbacks callbacks)
{
    return 0;
}

/*
 * Destroys an existing TimeWindow
 *  time_window: a pointer to the TimeWindow being destroyed
 */
void time_selection_menu_destroy(TimeSelectionWindow *time_selection_window)
{}

/*
 * Push the window onto the stack
 *  time_window: a pointer to the TimeWindow being pushed
 *  animated: whether to animate the push or not
 */
void time_selection_menu_push(TimeSelectionWindow *time_selection_window, bool animated)
{
    
}

/*
 * Pop the window off the stack
 *  time_window: a pointer to the TimeWindow to pop
 *  animated: whether to animate the pop or not
 */
void time_selection_menu_pop(TimeSelectionWindow *time_selection_window, bool animated)
{
    
}
