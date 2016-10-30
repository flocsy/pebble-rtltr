# pebble-rtltr
RTLTR for Pebble library enables developers to add an app-setting to switch between logical and visual character encoding to make apps/watchfaces display RTL texts correctly on both the original and on RTL enabled firmwares (like https://elbbeP.cpfx.ca/)

## Usage
```
#include "rtltr.h"

/**
 * Note: because RTLTR reverses the strings in place the strings need to be declared as `char *` and not as `const char *`.
 * You can collect your RTL strings in arrays or have them separately.
 */
char *example_hebrew_strings_array1[NUM_STRINGS1] = {
	"שלום",
	"להתראות"
};
char *example_hebrew_strings_array2[NUM_STRINGS2] = {
	"תשרי",
	"חשוון"
};
char *example_hebrew_string1 = "אחד";
char *example_hebrew_string2 = "שניים";

/**
  * If you already have a registered inbox_received callback, then you can add the call to rtltr_inbox_received_handler in it.
  * Or you can register rtltr_inbox_received_handler (see below in init_rtltr).
  */
static void app_inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // ...
  rtltr_inbox_received_handler(iterator, context);
}

/**
  * Optionally you can have a callback that will be called after RTLTR is enabled/disabled in the app-settings.
  */
void update_rtl_layers() {
  update_hebrew_date();
  update_hebrew_month_layer();
}

void init_rtltr(void) {
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // If you don't have a registered inbox_received callback yet, you can register rtltr_inbox_received_handler.
  // Or you can add the call to your existing callback (see above in app_inbox_received_callback).
  app_message_register_inbox_received(rtltr_inbox_received_handler);
  app_message_open(128, 128);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  // If you have your RTL strings in `char *str_arr[]`
  //
  // It is recommended to allocate the heap for the total number of arrays you have in your app
  // so that it's allocated once for all arrays and not reallocated and copied when adding another array.
  // Calling rtltr_ensure_registered_string_arrays_capacity is not necessary if you have only 1 array.
  rtltr_ensure_registered_string_arrays_capacity(2);
  //
  // Register the string arrays containing the RTL texts
  rtltr_register_string_array(example_hebrew_strings_array1, NUM_STRINGS1);
  rtltr_register_string_array(example_hebrew_strings_array2, NUM_STRINGS2);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  // If you have your RTL strings in `char *str_arr` variables
  //
  // It is recommended to allocate the heap for the total number of strings you have in your app
  // so that it's allocated once for all strings and not reallocated and copied when adding another string.
  // Calling rtltr_ensure_registered_strings_capacity is not necessary if you have only 1 string.
  rtltr_ensure_registered_strings_capacity(2);
  //
  // Register the strings containing the RTL texts
  rtltr_register_string(example_hebrew_string1);
  rtltr_register_string(example_hebrew_string1);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Optionally register a callback
  rtltr_register_callback_after_reverse_registered_strings(update_rtl_layers);

  // Finally load the settings. Without this your watch won't remember the RTLTR setting
  // and it will be disabled if you switch watchfaces or exit the app.
  rtltr_load_settings();
}

int main() {
  // init();
  // ...

  // In your main() or your init() you need to call init_rtltr.
  // If you register a callback using rtltr_register_callback_after_reverse_registered_strings
  // in init_rtltr then you need to set up your window BEFORE calling init_rtltr, otherwise you'll
  // get errors because of uninitialized window or layers.
  init_rtltr();
}
```

Note: RTLTR only works with single-line strings.
