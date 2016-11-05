/*
 * Copyright (c) 2016 Gavriel Fleischer <flocsy@gmail.com>
 */

#pragma once
#include <pebble.h>
#include "rtltr_config.h"

void rtltr_ensure_registered_string_arrays_capacity(size_t capacity);
void rtltr_register_string_array(char* const str_arr[], size_t str_arr_size);
void rtltr_ensure_registered_strings_capacity(size_t capacity);
void rtltr_register_string(char* const str);

void rtltr_strings_are_visual_encoded(void);
//void rtltr_reverse_string(const char* str);
void rtltr_reverse_registered_strings(void);
void rtltr_init(void);
void rtltr_free(void);

#if defined(RTLTR_INCLUDE_SETTINGS) && RTLTR_INCLUDE_SETTINGS == 1
//void rtltr_save_settings(void);
void rtltr_load_settings(void);
void rtltr_override(bool enable);
bool rtltr_is_overridden(void);
void rtltr_inbox_received_handler(DictionaryIterator *iter, void *context);
void rtltr_register_callback_after_reverse_registered_strings(void (*callback)(void));
#endif
