#include "LedControl.h"

#include <Adafruit_NeoPixel.h>

#include "pin.h"

static const uint8_t RGB_BRIGHTNESS = 32;

#ifndef RGB_LED_SK6812
#define RGB_LED_SK6812 0
#endif

#if RGB_LED_SK6812
static const neoPixelType RGB_LED_TYPE = NEO_GRBW + NEO_KHZ800;
#else
static const neoPixelType RGB_LED_TYPE = NEO_GRB + NEO_KHZ800;
#endif

static Adafruit_NeoPixel g_rgbLed(1, PIN_RGB_LED, RGB_LED_TYPE);

void LedControl::begin() {
    pinMode(PIN_LED_SIMPLE, OUTPUT);
    digitalWrite(PIN_LED_SIMPLE, LOW);

    g_rgbLed.begin();
    g_rgbLed.setBrightness(RGB_BRIGHTNESS);
    applyRgbColor();
}

void LedControl::toggleSimpleLed() {
    _simpleLedOn = !_simpleLedOn;
    digitalWrite(PIN_LED_SIMPLE, _simpleLedOn ? HIGH : LOW);
}

void LedControl::cycleRgbColor() {
    switch (_rgbColor) {
        case RgbColor::Off:
            _rgbColor = RgbColor::Red;
            break;
        case RgbColor::Red:
            _rgbColor = RgbColor::Green;
            break;
        case RgbColor::Green:
            _rgbColor = RgbColor::Blue;
            break;
        case RgbColor::Blue:
            _rgbColor = RgbColor::Off;
            break;
        default:
            _rgbColor = RgbColor::Off;
            break;
    }

    applyRgbColor();
}

const char* LedControl::rgbColorName() const {
    switch (_rgbColor) {
        case RgbColor::Off:
            return "OFF";
        case RgbColor::Red:
            return "RED";
        case RgbColor::Green:
            return "GREEN";
        case RgbColor::Blue:
            return "BLUE";
        default:
            return "OFF";
    }
}

void LedControl::applyRgbColor() {
    // Change these RGB values to create your own color palette.
    switch (_rgbColor) {
        case RgbColor::Off:
            g_rgbLed.setPixelColor(0, g_rgbLed.Color(0, 0, 0));
            break;
        case RgbColor::Red:
            g_rgbLed.setPixelColor(0, g_rgbLed.Color(255, 0, 0));
            break;
        case RgbColor::Green:
            g_rgbLed.setPixelColor(0, g_rgbLed.Color(0, 255, 0));
            break;
        case RgbColor::Blue:
            g_rgbLed.setPixelColor(0, g_rgbLed.Color(0, 0, 255));
            break;
        default:
            g_rgbLed.setPixelColor(0, g_rgbLed.Color(0, 0, 0));
            break;
    }

    g_rgbLed.show();
}
