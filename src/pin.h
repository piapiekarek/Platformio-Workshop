#pragma once

#include <Arduino.h>

// =============================================================================
// PIN MAPPING für RP2350
// =============================================================================
// Nur GPIO 0-9 und GP25-29 sind auf dem RP2350 verfügbar.
// Hier sind die Pins die wir verwenden:

static const uint8_t BUTTON_PIN = 0;      // Button an GPIO 0
static const uint8_t LED_PIN = 4;         // LED an GPIO 4
static const uint8_t TEMP_SENSOR_PIN = 3; // DS18B20 an GPIO 3

