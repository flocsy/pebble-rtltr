/*
 * Copyright (c) 2016 Gavriel Fleischer <flocsy@gmail.com>
 */

#pragma once

// In the distributions we include all optional code so anyone can use them straight away
// after `pebble package install pebble-rtltr`
// If you compile pebble-rtltr for your own usage you can turn off the irrelevant
// parts to make the compiled binary smaller.

#define RTLTR_INCLUDE_ARABIC 1
#define RTLTR_ENABLE_MULTILINE_STRINGS 1
#define RTLTR_INCLUDE_SETTINGS 1

// Persistent storage key for RTLTR enabled/disabled setting
#define RTLTR_KEY 42
