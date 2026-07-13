// =============================================================================
//  Example: WS2812 LED strip (FastLED)
// =============================================================================
//
//  Lights up a chain of 16 WS2812 LEDs: first a one-by-one startup chase,
//  then a moving rainbow.
//  Adjust LED_COUNT below to match the number of LEDs you actually have.
//  DATA pin comes from pin.h (RGB_LED_PIN).
// =============================================================================

#include <Arduino.h>
#include <FastLED.h>
#include "pin.h"

const int LED_COUNT = 16; // number of LEDs in this chain

CRGB leds[LED_COUNT];

void setup() {
    FastLED.addLeds<WS2812, RGB_LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.setBrightness(80); // 0-255, avoid blinding brightness and high power draw

    // Startup chase: light up one LED at a time until the whole strip is lit.
    // A while loop fits here since we just keep going "while not all lit yet".
    int litCount = 0;
    while (litCount < LED_COUNT) {
        leds[litCount] = CRGB::White;
        FastLED.show();
        delay(80);
        litCount++;
    }
    delay(300);
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
