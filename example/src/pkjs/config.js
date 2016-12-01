/**
  * Copyright (c) 2016 Gavriel Fleischer <flocsy@gmail.com>
  *
  * Use `pebble package install pebble-clay` to add pebble-clay to your app.
  * See: index.js
  * This is a simple configuration with 1 toggle to enable/disable RTLTR.
  *
  * IMPORTANT: You need to keep the `"messageKey": "RTLTR_OVERRIDE"` to work. All the rest can be changed.
  */

const RTLTR_CONFIG = require('pebble-rtltr/config');

module.exports = [
  {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Your settings"
      },
      // ...
    ]
  },
  RTLTR_CONFIG,
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
