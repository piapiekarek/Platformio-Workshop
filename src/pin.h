#pragma once

#include <Arduino.h>

// =============================================================================
// FEATURES ON/OFF
// =============================================================================
// Not every group has an RC522 RFID reader wired up. Set to 0 to remove all
// RFID code from the program (src/Rfid.cpp).
#define RFID_ENABLED 1

// Not every group has a WS2812 LED strip wired up. Set to 0 to remove all
// RGB LED code from the program (src/RgbLed.cpp).
#define RGB_LED_ENABLED 1

// =============================================================================
// PIN MAPPING for RP2350
// =============================================================================
// Only GPIO 0-9 and GP25-29 are available on the RP2350 board.
// Here are the pins we use:

static const uint8_t BUTTON_PIN = 1;      // Button on GPIO 1
static const uint8_t LED_PIN = 4;         // LED on GPIO 4
static const uint8_t TEMP_SENSOR_PIN = 3; // DS18B20 on GPIO 3

static const uint8_t RGB_LED_PIN = 2;     // WS2812 strip (DATA) on GPIO 2
static const uint8_t RGB_LED_COUNT = 8;   // number of LEDs in the strip - adjust to your hardware

static const uint8_t RFID_SCK_PIN = 6;    // RC522 SPI SCK on GPIO 6 (SPI0, GPIO2/6/18/22 are the only valid SCK pins)
static const uint8_t RFID_MOSI_PIN = 7;   // RC522 SPI MOSI on GPIO 7 (SPI0, GPIO3/7/19/23 are the only valid MOSI pins)
static const uint8_t RFID_MISO_PIN = 0;   // RC522 SPI MISO on GPIO 0 (SPI0)
static const uint8_t RFID_SS_PIN = 5;     // RC522 SS/SDA on GPIO 5 (SPI0)
static const uint8_t RFID_RST_PIN = 8;    // RC522 RST on GPIO 8 (not an SPI function pin, any free GPIO works)
