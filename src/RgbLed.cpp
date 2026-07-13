#include "RgbLed.h"
#include "pin.h"

#if RGB_LED_ENABLED

#include <FastLED.h>

namespace {
    CRGB leds[RGB_LED_COUNT];
}

void rgbLedSetup() {
    FastLED.addLeds<WS2812, RGB_LED_PIN, GRB>(leds, RGB_LED_COUNT);
    FastLED.setBrightness(80); // 0-255, avoid blinding brightness and high power draw
    rgbLedOff();
}

void rgbLedSetColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < RGB_LED_COUNT) {
        leds[index] = CRGB(r, g, b);
    }
}

void rgbLedFill(uint8_t r, uint8_t g, uint8_t b) {
    fill_solid(leds, RGB_LED_COUNT, CRGB(r, g, b));
}

void rgbLedOff() {
    rgbLedFill(0, 0, 0);
    rgbLedShow();
}

void rgbLedShow() {
    FastLED.show();
}

#else // RGB_LED_ENABLED == 0: everything becomes a no-op, main.cpp needs no special cases

void rgbLedSetup() {}
void rgbLedSetColor(uint8_t, uint8_t, uint8_t, uint8_t) {}
void rgbLedFill(uint8_t, uint8_t, uint8_t) {}
void rgbLedOff() {}
void rgbLedShow() {}

#endif
