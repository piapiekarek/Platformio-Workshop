// =============================================================================
//  Example: WS2812 LED strip (FastLED)
// =============================================================================
//
//  Lights up a chain of 10 WS2812 LEDs with a moving rainbow.
//  Adjust LED_COUNT below to match the number of LEDs you actually have.
//  DATA pin comes from pin.h (RGB_LED_PIN).
// =============================================================================

#include <Arduino.h>
#include <FastLED.h>
#include "pin.h"

const int LED_COUNT = 10; // number of LEDs in this chain

CRGB leds[LED_COUNT];

void setup() {
    FastLED.addLeds<WS2812, RGB_LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.setBrightness(80); // 0-255, avoid blinding brightness and high power draw
}

void loop() {
    static uint8_t hue = 0;

    for (int i = 0; i < LED_COUNT; i++) {
        leds[i] = CHSV(hue + i * 12, 255, 255); // rainbow spread across the strip
    }
    FastLED.show();

    hue++;
    delay(30);
}
