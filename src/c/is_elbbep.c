/*
 * Copyright (c) 2016 Gavriel Fleischer <flocsy@gmail.com>
 */

#include "is_elbbep.h"

#define MAGIC_ELBBEP_OVERFLOW_MODE ((GTextOverflowMode)0xE5);
static const char * const arabic_string = "ل";

bool is_elbbep() {
  static bool ran = false;
  static bool is_elbbep;
  if (!ran) {
    ran = true;
    char * str = malloc(strlen(arabic_string)+1);
    strcpy(str, arabic_string);
    GFont font = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
    GRect bounds = GRect(10, 10, 10, 10);
    graphics_text_layout_get_content_size(str, font, bounds, (GTextOverflowMode)0xE5, GTextAlignmentCenter);
    is_elbbep = strcmp(arabic_string, str);
    free(str);
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "is_elbbep: %d", is_elbbep);
  return is_elbbep;
}
