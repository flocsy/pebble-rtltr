/**
  * Copyright (c) 2016 Gavriel Fleischer <flocsy@gmail.com>
  *
  * Use `pebble package install pebble-clay` to add pebble-clay to your app.
  * See: index.js
  * This is a simple configuration with 1 toggle to enable/disable RTLTR.
  *
  * IMPORTANT: You need to keep the `"messageKey": "RTLTR_OVERRIDE"` to work. All the rest can be changed.
  */

module.exports = {
    "type": "section",
    "items": [
        {
            "type": "heading",
            "defaultValue": "Language Settings"
        },
        {
            "type": "text",
            "defaultValue": "Override RTLTR if Hebrew/Arabic texts are displayed in the incorrect direction."
        },
        {
            "type": "toggle",
            "messageKey": "RTLTR_OVERRIDE",
            "label": "Override RTLTR",
            "defaultValue": false
        }
    ]
};
