#pragma once

#include <Arduino.h>

// Central pin mapping for external components.
// Only GP0-9 and GP25-29 are exposed on the RP2350 board.
static const uint8_t PIN_BUTTON_1 = 0;
// static const uint8_t PIN_BUTTON_2 = 1;
// static const uint8_t PIN_BUTTON_3 = 2;
static const uint8_t PIN_DS18B20 = 3;
static const uint8_t PIN_LED_SIMPLE = 4;
static const uint8_t PIN_RGB_LED = 5;
static const uint8_t PIN_SERVO = 6;
