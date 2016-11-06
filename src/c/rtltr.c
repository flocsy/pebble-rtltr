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

static bool rtltr_are_strings_logical_encoded = true;
static size_t rtltr_registered_strings_capacity = 0;
static int rtltr_registered_strings_index = -1;
static size_t rtltr_registered_string_arrays_capacity = 0;
static size_t rtltr_registered_string_arrays_size = 0;
static StringArray *rtltr_registered_string_arrays = NULL;

#if defined(RTLTR_INCLUDE_SETTINGS) && RTLTR_INCLUDE_SETTINGS == 1
static bool rtltr_is_overridden_from_settings = false;
static void (*rtltr_callback_after_reverse_registered_strings)(void) = NULL;
#endif

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

void rtltr_register_string_array(char* const str_arr[], size_t str_arr_size) {
  rtltr_ensure_registered_string_arrays_capacity(rtltr_registered_string_arrays_size+1);
  rtltr_registered_string_arrays[rtltr_registered_string_arrays_size].arr = (char**)str_arr;
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

void rtltr_register_string(char* const str) {
  rtltr_ensure_registered_strings_capacity(rtltr_registered_strings_index >-1 ? rtltr_registered_string_arrays[rtltr_registered_strings_index].size + 1 : 1);
  rtltr_registered_string_arrays[rtltr_registered_strings_index].arr[rtltr_registered_string_arrays[rtltr_registered_strings_index].size++] = str;
}

void rtltr_strings_are_visual_encoded(void) {
  rtltr_are_strings_logical_encoded = false;
}

void rtltr_reverse_string(char* const str) {
  if (str && *str) {
#if defined(RTLTR_ENABLE_MULTILINE_STRINGS) && RTLTR_ENABLE_MULTILINE_STRINGS == 1
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "rtltr_reverse_string(multiline): before: %p -> %s", str, str);
    const char *start = (const char*)str;
    char *end;
    do {
      end = strchr(start, '\n');
      if (!end) {
        end = strchr(start, '\0');
      }
      rtl_apply((char*)start, end-1);
      start = end + 1;
    } while (*end != '\0');
#else
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "rtltr_reverse_string: before: %p -> %s", str, str);
    rtl_apply(str, str + strlen(str) - 1);
#endif
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "rtltr_reverse_string: after: %p -> %s", str, str);
  }
}

void rtltr_reverse_registered_strings(void) {
  if ((rtltr_are_strings_logical_encoded
#if defined(RTLTR_INCLUDE_SETTINGS) && RTLTR_INCLUDE_SETTINGS == 1
                                         != rtltr_is_overridden_from_settings
#endif
                                                                             ) ^ is_elbbep()) {
    rtltr_are_strings_logical_encoded = !rtltr_are_strings_logical_encoded;
    for (size_t a = 0; a < rtltr_registered_string_arrays_size; a++) {
      for (size_t s = 0; s < rtltr_registered_string_arrays[a].size; s++) {
        rtltr_reverse_string(rtltr_registered_string_arrays[a].arr[s]);
      }
    }
#if defined(RTLTR_INCLUDE_SETTINGS) && RTLTR_INCLUDE_SETTINGS == 1
    if (rtltr_callback_after_reverse_registered_strings != NULL) {
      (*rtltr_callback_after_reverse_registered_strings)();
    }
#endif
  }
}

void rtltr_init(void) {
  rtltr_reverse_registered_strings();
}

static void rtltr_free_registered_strings(void) {
  if (rtltr_registered_strings_index != -1) {
    free(rtltr_registered_string_arrays[rtltr_registered_strings_index].arr);
    rtltr_registered_string_arrays[rtltr_registered_strings_index].arr = NULL;
    rtltr_registered_string_arrays[rtltr_registered_strings_index].size = 0;
    rtltr_registered_strings_capacity = 0;
    rtltr_registered_strings_index = -1;
  }
}

static void rtltr_free_registered_string_arrays(void) {
  if (rtltr_registered_string_arrays != NULL) {
    free(rtltr_registered_string_arrays);
    rtltr_registered_string_arrays = NULL;
    rtltr_registered_string_arrays_size = 0;
    rtltr_registered_string_arrays_capacity = 0;
  }
}

void rtltr_free(void) {
  rtltr_free_registered_strings();
  rtltr_free_registered_string_arrays();
}

#if defined(RTLTR_INCLUDE_SETTINGS) && RTLTR_INCLUDE_SETTINGS == 1

// Save the settings to persistent storage
static void rtltr_save_settings(void) {
  persist_write_bool(RTLTR_KEY, rtltr_is_overridden_from_settings);
}

void rtltr_load_settings(void) {
  if (persist_exists(RTLTR_KEY)) {
    rtltr_is_overridden_from_settings = persist_read_bool(RTLTR_KEY);
  }
  // we always need to run reverse once in order for the is_elbbep detection to kick in
  rtltr_reverse_registered_strings();
}

void rtltr_override(bool enable) {
  rtltr_is_overridden_from_settings = enable;
  rtltr_save_settings();
  rtltr_reverse_registered_strings();
}

bool rtltr_is_overridden(void) {
  return rtltr_is_overridden_from_settings;
}

// AppMessage receive handler
void rtltr_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Read boolean preferences
  Tuple *rtltr_t = dict_find(iter, MESSAGE_KEY_RTLTR_OVERRIDE);
  if (rtltr_t) {
    rtltr_override(rtltr_t->value->int32 == 1);
  }
}

void rtltr_register_callback_after_reverse_registered_strings(void (*callback)(void)) {
  rtltr_callback_after_reverse_registered_strings = callback;
}

#endif
