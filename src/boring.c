#include <pebble.h>
#include "pebble-assist.h"

static void init(void);
static void deinit(void);
static void window_load(Window* window);
static void window_unload(Window* window);
static void layer_main_render(Layer* layer, GContext* ctx);
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);

static Window* window;
static Layer* layer_main;
static GFont* font_bebas;

int main(void) {
  init();
  app_event_loop();
  deinit();
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  font_bebas = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BEBAS_60));
}

static void deinit(void) {
  window_destroy(window);
  fonts_unload_custom_font(font_bebas);
}

static void window_load(Window *window) {
  layer_main = layer_create_fullscreen(window);
  layer_set_update_proc(layer_main, layer_main_render);
  layer_add_to_window(layer_main, window);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void window_unload(Window *window) {
  layer_destroy(layer_main);
}

static void layer_main_render(Layer* layer, GContext* ctx) {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char buffer[] = "00:00";

  if (clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  }
  else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  graphics_context_set_fill_color(ctx, GColorBulgarianRose);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);

  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, buffer, font_bebas, GRect(4, 58, 144, 60), GTextOverflowModeFill, GTextAlignmentCenter, NULL);

  graphics_context_set_text_color(ctx, GColorDarkGray);
  graphics_draw_text(ctx, buffer, font_bebas, GRect(1, 55, 144, 60), GTextOverflowModeFill, GTextAlignmentCenter, NULL);

  graphics_context_set_text_color(ctx, GColorLightGray);
  graphics_draw_text(ctx, buffer, font_bebas, GRect(-2, 52, 144, 60), GTextOverflowModeFill, GTextAlignmentCenter, NULL);

  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_draw_text(ctx, buffer, font_bebas, GRect(-5, 49, 144, 60), GTextOverflowModeFill, GTextAlignmentCenter, NULL);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(layer_main);
}
