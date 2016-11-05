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
pebble package install pebble-rtltr
```

## Usage

### Include rtltr.h

```c
#include "pebble-rtltr/rtltr.h"
```

### Define the RTL strings
**Note**: Because RTLTR reverses the strings in place the strings need to be declared as `char[]` and not as `char*`.
If you use `char*` then the compiler might compile the strings overlapped and reversing them will result in chaos
or even crash.
You can collect your RTL strings in arrays or have them separately.

```c
char example_hebrew_strings_array1[NUM_STRINGS1][14] = {
	"שלום",
	"להתראות"
};
char tishrey[]="תשרי", hesvan[]="חשוון";
char *example_hebrew_strings_array2[NUM_STRINGS2] = {
	tishrey,
	hesvan
};
char example_hebrew_string1[] = "אחד";
char example_hebrew_string2[] = "שניים";
```

### Register your string arrays
If you have your RTL strings in arrays like `char *str_arr[]` or `char str_arr[][]`:

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

### Register your strings
If you have your RTL strings in variables like `char str[]`:

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

### Set the encoding
Logical encoding is the default in pebble-rtltr. We recommend you to have all your strings logically encoded,
the way you see them correctly in modern editors (UTF-8 or ISO-8859-8-I "שלום", "سلم").
If your strings are logically-encoded you should skip this step.

However if the strings in your code are visually encoded (ISO-8859-8 "םולש", "ملس") instead of the
recommended logical call rtltr_strings_are_visual_encoded before calling rtltr_load_settings.

```c
rtltr_strings_are_visual_encoded();
```

### Init RTLTR
Finally initialize RTLTR.

```c
rtltr_init();
```

### Free resources
If you're not using app-settings to override RTLTR, then the reversing of your strings will only take place
once, in `rtltr_init()`, so you can call `rtltr_free()` right after `rtltr_init()`.
```c
rtltr_free();
```

If you're using app-settings to override RTLTR, then you can't free the resources before the end of your app, 'cause at any time the user can go to settings to change the RTLTR_OVERRIDE, so you need to free the used resources in `deinit()`
```c
void deinit() {
  rtltr_free();
  // ...
}
```

### Finally you need to call app_init_rtltr from the app's init
Put all the initialization steps in one method and call it from main or init.
In your `main()` or `init()` you need to call `app_init_rtltr()`.
If you register a callback using `rtltr_register_callback_after_reverse_registered_strings()`
in `app_init_rtltr()` then you need to set up your window and layers BEFORE calling
`app_init_rtltr()`, otherwise you'll get errors because of uninitialized window or layers.

```c
void init() {
  // ...
  app_init_rtltr();
}
```

**Note**: If you're not using app-settings to override RTLTR, then the reversing of your strings will only take place
once, in rtltr_init, so you can call `rtltr_free()` right after `rtltr_init()`.

### Optionally, if you want to use app-settings to be able to override RTLTR

#### Register inbox_received callback
If you already have a registered inbox_received callback, then you can add the call to `rtltr_inbox_received_handler` in it:

```c
void app_inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // ...
  rtltr_inbox_received_handler(iterator, context);
}
```

Or if you don't have a registered inbox_received callback yet, you can register `rtltr_inbox_received_handler`:

```c
app_message_register_inbox_received(rtltr_inbox_received_handler);
app_message_open(128, 128);
```

#### Optionally you can have a callback that will be called after RTLTR is enabled/disabled in the app-settings.

```c
void app_update_rtl_layers() {
  // Redraw layers affected by reversing the strings
  app_update_hebrew_date();
  app_update_hebrew_month_layer();
}
```

And register it in `app_init_rtltr()`:
```c
rtltr_register_callback_after_reverse_registered_strings(app_update_rtl_layers);
```

#### Load the settings
Finally before `rtltr_init()` you need to load the settings. Without this your watch won't remember the RTLTR setting
and it will be disabled if you switch watchfaces or exit the app.
```c
rtltr_load_settings();
rtltr_init();
```

### elbbeP firmware detection

```c
#include "is_elbbep.h"

if (is_elbbep()) {...}
```

## Examples

### Strings are logical encoded and in arrays, no app-settings
```c
void app_init_rtltr(void) {
  rtltr_ensure_registered_string_arrays_capacity(2);
  rtltr_register_string_array(example_hebrew_strings_array1, NUM_STRINGS1);
  rtltr_register_string_array(example_hebrew_strings_array2, NUM_STRINGS2);
  rtltr_init();
  rtltr_free();
}
```

### Strings are logical encoded and in individal variables, no app-settings
```c
void app_init_rtltr(void) {
  rtltr_ensure_registered_strings_capacity(2);
  rtltr_register_string(example_hebrew_string1);
  rtltr_register_string(example_hebrew_string1);
  rtltr_init();
  rtltr_free();
}
```

### You can have both some of the strings in arrays and others individually registered
```c
void app_init_rtltr(void) {
  rtltr_ensure_registered_string_arrays_capacity(2);
  rtltr_register_string_array(example_hebrew_strings_array1, NUM_STRINGS1);
  rtltr_register_string_array(example_hebrew_strings_array2, NUM_STRINGS2);

  rtltr_ensure_registered_strings_capacity(2);
  rtltr_register_string(example_hebrew_string1);
  rtltr_register_string(example_hebrew_string1);

  rtltr_init();
  rtltr_free();
}
```

### If strings are VISUAL encoded call `rtltr_strings_are_visual_encoded()`, no app-settings
```c
void app_init_rtltr(void) {
  rtltr_ensure_registered_string_arrays_capacity(2);
  rtltr_register_string_array(example_hebrew_strings_array1, NUM_STRINGS1);
  rtltr_register_string_array(example_hebrew_strings_array2, NUM_STRINGS2);

  // If the strings in your code are visually encoded (ISO-8859-8 "םולש", "ملس") instead of the
  // recommended logical (ISO-8859-8-I "שלום", "سلم")
  // call rtltr_strings_are_visual_encoded before calling rtltr_load_settings
  rtltr_strings_are_visual_encoded();

  rtltr_init();
  rtltr_free();
}
```

### If you want to support overriding RTLTR from app-settings
```c
void app_update_rtl_layers() {
  // Redraw layers affected by reversing the strings
  app_update_hebrew_date();
  app_update_hebrew_month_layer();
}

void app_init_rtltr(void) {
  rtltr_ensure_registered_string_arrays_capacity(2);
  rtltr_register_string_array(example_hebrew_strings_array1, NUM_STRINGS1);
  rtltr_register_string_array(example_hebrew_strings_array2, NUM_STRINGS2);

  app_message_register_inbox_received(rtltr_inbox_received_handler);
  app_message_open(128, 128);
  rtltr_register_callback_after_reverse_registered_strings(app_update_rtl_layers);
  rtltr_load_settings();

  rtltr_init();
}

void init() {
  // ...
  app_init_rtltr();
}

void deinit() {
  rtltr_free();
  // ...
}

int main() {
  init();
  app_event_loop();
  deinit();
}
```

## License

GNU GENERAL PUBLIC LICENSE 3.0

## Credits

pebble-rtltr library was written by Gavriel Fleischer [pebble-rtltr](https://github.com/flocsy/pebble-rtltr)

pebble-rtltr is based on code from Collin Fair [elbbeP](https://github.com/cpfair/elbbep)

## Change log

#### 0.3.0

* fixed reversing multi-line strings
* moved compile-time config to rtltr_config.h
* added `rtltr_free()`
* added examples to README

#### 0.2.0

* automagically detect if running on elbbeP firmware.

#### 0.1.0

* first release.
