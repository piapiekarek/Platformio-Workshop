#pragma once

#include <Arduino.h>

// Drives a WS2812 LED strip (addressable RGB LEDs) using FastLED.
// For groups without a WS2812 strip: pin.h -> set RGB_LED_ENABLED to 0,
// and this module does nothing anymore. main.cpp does not need to be
// changed for that.
//
// Pin and LED count live in pin.h (RGB_LED_PIN, RGB_LED_COUNT).

// Call once in setup().
void rgbLedSetup();

// Set a single LED (index 0 = first LED in the strip).
// Only takes effect after the next rgbLedShow().
void rgbLedSetColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

// Set all LEDs to the same color. Only takes effect after the next rgbLedShow().
void rgbLedFill(uint8_t r, uint8_t g, uint8_t b);

// Turn all LEDs off immediately.
void rgbLedOff();

// Must be called after rgbLedSetColor()/rgbLedFill() to actually make the
// new color visible on the strip.
void rgbLedShow();
