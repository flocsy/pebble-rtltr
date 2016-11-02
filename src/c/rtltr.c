/*
 * Copyright (c) 2016 Gavriel Fleischer <flocsy@gmail.com>
 */

#include "is_elbbep.h"
#include "rtltr.h"
#include "rtl.h"

typedef struct StringArray {
  char ** arr;
  size_t size;
} StringArray;

static bool is_rtltr_setting_on = false;
static bool is_rtltr_applied = false;
static size_t rtltr_registered_strings_capacity = 0;
static int rtltr_registered_strings_index = -1;
static size_t rtltr_registered_string_arrays_capacity = 0;
static size_t rtltr_registered_string_arrays_size = 0;
static StringArray *rtltr_registered_string_arrays = NULL;
static void (*rtltr_callback_after_reverse_registered_strings)(void) = NULL;

// Save the settings to persistent storage
static void rtltr_save_settings() {
  persist_write_bool(RTLTR_KEY, is_rtltr_setting_on);
}

void rtltr_load_settings() {
  if (persist_exists(RTLTR_KEY)) {
    is_rtltr_setting_on = persist_read_bool(RTLTR_KEY);
  }
  // we always need to run reverse once in order for the is_elbbep detection to kick in
  rtltr_reverse_registered_strings();
}

void rtltr_enable(bool enable) {
  is_rtltr_setting_on = enable;
  rtltr_save_settings();
  rtltr_reverse_registered_strings();
}

bool rtltr_is_enabled() {
  return is_rtltr_setting_on;
}

// AppMessage receive handler
void rtltr_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Read boolean preferences
  Tuple *rtltr_t = dict_find(iter, MESSAGE_KEY_ENABLE_RTLTR);
  if (rtltr_t) {
    rtltr_enable(rtltr_t->value->int32 == 1);
  }
}

void rtltr_ensure_registered_string_arrays_capacity(size_t capacity) {
  if (rtltr_registered_string_arrays_capacity < capacity) {
    if (rtltr_registered_string_arrays == NULL) {
      rtltr_registered_string_arrays = calloc(capacity, sizeof(StringArray));
    } else {
      rtltr_registered_string_arrays = realloc(rtltr_registered_string_arrays, capacity * sizeof(StringArray));
    }
    rtltr_registered_string_arrays_capacity = capacity;
  }
}

void rtltr_register_string_array(char* str_arr[], size_t str_arr_size) {
  rtltr_ensure_registered_string_arrays_capacity(rtltr_registered_string_arrays_size+1);
  rtltr_registered_string_arrays[rtltr_registered_string_arrays_size].arr = str_arr;
  rtltr_registered_string_arrays[rtltr_registered_string_arrays_size].size = str_arr_size;
  rtltr_registered_string_arrays_size++;
}

void rtltr_ensure_registered_strings_capacity(size_t capacity) {
  if (rtltr_registered_strings_index == -1) {
    rtltr_registered_strings_index = rtltr_registered_string_arrays_size;
    rtltr_register_string_array(NULL, 0);
  }
  if (rtltr_registered_strings_capacity < capacity) {
    if (rtltr_registered_string_arrays[rtltr_registered_strings_index].arr == NULL) {
      rtltr_registered_string_arrays[rtltr_registered_strings_index].arr = calloc(capacity, sizeof(char*));
      rtltr_registered_string_arrays[rtltr_registered_strings_index].size = 0;
    } else {
      rtltr_registered_string_arrays[rtltr_registered_strings_index].arr =
        realloc(rtltr_registered_string_arrays[rtltr_registered_strings_index].arr, capacity * sizeof(char*));
    }
    rtltr_registered_strings_capacity = capacity;
  }
}

void rtltr_register_string(char* str) {
  rtltr_ensure_registered_strings_capacity(rtltr_registered_strings_index >-1 ? rtltr_registered_string_arrays[rtltr_registered_strings_index].size + 1 : 1);
  rtltr_registered_string_arrays[rtltr_registered_strings_index].arr[rtltr_registered_string_arrays[rtltr_registered_strings_index].size++] = str;
}

void rtltr_reverse_string(const char* str) {
  if (str && *str != 0) {
    rtl_apply((char*)str, (char*)(str+strlen(str)-1));
  }
}

void rtltr_reverse_registered_strings() {
  if ((is_rtltr_applied != is_rtltr_setting_on) ^ is_elbbep()) {
    is_rtltr_applied = is_rtltr_setting_on;
    for (size_t a = 0; a < rtltr_registered_string_arrays_size; a++) {
      for (size_t s = 0; s < rtltr_registered_string_arrays[a].size; s++) {
        rtltr_reverse_string(rtltr_registered_string_arrays[a].arr[s]);
      }
    }
    if (rtltr_callback_after_reverse_registered_strings != NULL) {
      (*rtltr_callback_after_reverse_registered_strings)();
    }
  }
}

void rtltr_register_callback_after_reverse_registered_strings(void (*callback)(void)) {
  rtltr_callback_after_reverse_registered_strings = callback;
}
