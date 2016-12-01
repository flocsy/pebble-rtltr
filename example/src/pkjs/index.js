/**
  * Copyright (c) 2016 Gavriel Fleischer <flocsy@gmail.com>
  *
  * Use `pebble package install pebble-clay` to add pebble-clay to your app.
  * This is a simple example, add these lines to your app's main js.
  * See: config.js
  */

// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);
