# pebble-rtltr

RTLTR for Pebble - create apps/watchfaces that display RTL on every watch regardless of the firmware

## Description

RTLTR for Pebble library enables developers to add an app-setting to switch between logical and visual character encoding to make apps/watchfaces display RTL texts correctly on both the original and on RTL enabled firmwares (like https://elbbeP.cpfx.ca/)

## Badges

[![NPM](https://nodei.co/npm/pebble-rtltr.png?downloads=true&stars=true)](https://nodei.co/npm/pebble-rtltr/)

[![NPM](https://nodei.co/npm-dl/pebble-rtltr.png?months=1)](https://nodei.co/npm/pebble-rtltr/)

![dependencies](https://david-dm.org/flocsy/pebble-rtltr.png)

## Installation

```
pebble package install pebble-rtltr --save
```

## Usage

Because RTLTR reverses the strings in place the strings need to be declared as `char *` and not as `const char *`.
You can collect your RTL strings in arrays or have them separately.

```c
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
```

Don't forget to include rtltr.h

```c
#include "rtltr.h"
```

If you already have a registered inbox_received callback, then you can add the call to rtltr_inbox_received_handler in it.

```c
void app_inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // ...
  rtltr_inbox_received_handler(iterator, context);
}
```

Or if you don't have a registered inbox_received callback yet, you can register rtltr_inbox_received_handler.

```c
app_message_register_inbox_received(rtltr_inbox_received_handler);
app_message_open(128, 128);
```

Optionally you can have a callback that will be called after RTLTR is enabled/disabled in the app-settings.

```c
void update_rtl_layers() {
  update_hebrew_date();
  update_hebrew_month_layer();
  // ...
}
```
And register it:
```c
rtltr_register_callback_after_reverse_registered_strings(update_rtl_layers);
```

If you have your RTL strings in arrays like `char *str_arr[]`

It is recommended to allocate the heap for the total number of arrays you have in your app before
the 1st call to `rtltr_register_string_array`, so that it's allocated once for all arrays and not reallocated
and copied when adding another array.
Calling `rtltr_ensure_registered_string_arrays_capacity` is not necessary if you have only 1 array.

```c
rtltr_ensure_registered_string_arrays_capacity(2);
```

Register the string arrays containing the RTL texts.

```c
rtltr_register_string_array(example_hebrew_strings_array1, NUM_STRINGS1);
rtltr_register_string_array(example_hebrew_strings_array2, NUM_STRINGS2);
```

If you have your RTL strings in variables like `char *str_arr`

It is recommended to allocate the heap for the total number of strings you have in your app before
the 1st call to `rtltr_register_string`, so that it's allocated once for all strings and not reallocated
and copied when adding another string.
Calling rtltr_ensure_registered_strings_capacity is not necessary if you have only 1 string.

```c
rtltr_ensure_registered_strings_capacity(2);
```

Register the strings containing the RTL texts

```c
rtltr_register_string(example_hebrew_string1);
rtltr_register_string(example_hebrew_string1);
```

Finally load the settings. Without this your watch won't remember the RTLTR setting
and it will be disabled if you switch watchfaces or exit the app.

```c
rtltr_load_settings();
```

So putting it all together in a single method:

```c
void init_rtltr(void) {
  // If you don't have a registered inbox_received callback yet, you can register rtltr_inbox_received_handler.
  // Or you can add the call to your existing callback (see above in app_inbox_received_callback).
  app_message_register_inbox_received(rtltr_inbox_received_handler);
  app_message_open(128, 128);

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

  // Optionally register a callback
  rtltr_register_callback_after_reverse_registered_strings(update_rtl_layers);

  // Finally load the settings. Without this your watch won't remember the RTLTR setting
  // and it will be disabled if you switch watchfaces or exit the app.
  rtltr_load_settings();
}
```

Finally you need to call init_rtltr from the app's main init:

```c
int main() {
  // init();
  // ...

  // In your main() or your init() you need to call init_rtltr.
  // If you register a callback using rtltr_register_callback_after_reverse_registered_strings
  // in init_rtltr then you need to set up your window and layers BEFORE calling init_rtltr,
  // otherwise you'll get errors because of uninitialized window or layers.
  init_rtltr();
}
```

## License

GNU GENERAL PUBLIC LICENSE 3.0

## Credits

pebble-rtltr library was written by Gavriel Fleischer [pebble-rtltr](https://github.com/flocsy/pebble-rtltr)

pebble-rtltr is based on code from Collin Fair [elbbeP](https://github.com/cpfair/elbbep)

## Note

RTLTR only works with single-line strings.

## Change log

#### 0.2.0

* automagically detect if running on elbbeP firmware.

#### 0.1.0

* first release.
