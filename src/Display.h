#pragma once
#include <Arduino.h>

#ifdef USE_ARDUINO_GFX
#include <Arduino_GFX_Library.h>
#else
#include <TFT_eSPI.h>
#endif

// ═══════════════════════════════════════════════════════════════════════════
//  Display – simple class for drawing on the Waveshare 1.47" LCD
// ═══════════════════════════════════════════════════════════════════════════
//
//  Colour constants ready to use:
//
//    BLACK   WHITE   RED     GREEN   BLUE
//    YELLOW  CYAN    MAGENTA GRAY    ORANGE
//
//  All methods use pixel coordinates.
//  The display is 320 pixels wide and 172 pixels tall (landscape).
//
//        (0,0) ──────────────────────── (319,0)
//          │                                │
//          │                                │
//        (0,171) ────────────────── (319,171)
//
// ═══════════════════════════════════════════════════════════════════════════

// ── Colour constants ───────────────────────────────────────────────────────
#ifdef USE_ARDUINO_GFX
static const uint16_t BLACK   = 0x0000;
static const uint16_t WHITE   = 0xFFFF;
static const uint16_t RED     = 0xF800;
static const uint16_t GREEN   = 0x07E0;
static const uint16_t BLUE    = 0x001F;
static const uint16_t YELLOW  = 0xFFE0;
static const uint16_t CYAN    = 0x07FF;
static const uint16_t MAGENTA = 0xF81F;
static const uint16_t GRAY    = 0x7BEF;
static const uint16_t ORANGE  = 0xFD20;
#else
static const uint16_t BLACK   = TFT_BLACK;
static const uint16_t WHITE   = TFT_WHITE;
static const uint16_t RED     = TFT_RED;
static const uint16_t GREEN   = TFT_GREEN;
static const uint16_t BLUE    = TFT_BLUE;
static const uint16_t YELLOW  = TFT_YELLOW;
static const uint16_t CYAN    = TFT_CYAN;
static const uint16_t MAGENTA = TFT_MAGENTA;
static const uint16_t GRAY    = 0x7BEF;
static const uint16_t ORANGE  = TFT_ORANGE;
#endif

// Darker variants for text on a light (white) background.
// The "normal" colors above read well on a black background (see examples/),
// but on white they are too pale/light for text.
// Rule of thumb: text on WHITE -> use the DARK* variant.
static const uint16_t DARKGREEN   = 0x0360; // instead of GREEN
static const uint16_t DARKRED     = 0x90A2; // instead of RED
static const uint16_t DARKCYAN    = 0x036F; // instead of CYAN
static const uint16_t DARKMAGENTA = 0x780F; // instead of MAGENTA
static const uint16_t DARKGRAY    = 0x5ACB; // instead of GRAY
static const uint16_t DARKORANGE  = 0xAA80; // instead of ORANGE

// ── Display class ──────────────────────────────────────────────────────────
class Display {
public:
    // Initialise the display – call once in setup()
    void begin();

    // Set display orientation (0..3)
    void setRotation(uint8_t rotation);

    // Fill the whole screen with one colour
    // Example: display.clear(BLACK);
    void clear(uint16_t color = BLACK);

    // Print a text string
    // Example: display.print("Hello!", 10, 10, WHITE, 2);
    //   x, y  = position (top-left corner of the text)
    //   color = text colour
    //   size  = 1 (small) to 4 (very large)
    void print(const String& text, int x, int y,
               uint16_t color = WHITE, uint8_t size = 2);

    // Print a number (e.g. a sensor reading)
    // Example: display.number(temperature, 10, 50, RED);
    void number(float value, int x, int y,
                uint16_t color = WHITE, uint8_t size = 2,
                uint8_t decimals = 1);

    // Draw a rectangle outline
    // Example: display.rect(10, 10, 100, 50, BLUE);
    void rect(int x, int y, int width, int height, uint16_t color = WHITE);

    // Draw a filled rectangle
    // Example: display.rectFilled(10, 10, 100, 50, GREEN);
    void rectFilled(int x, int y, int width, int height, uint16_t color = WHITE);

    // Draw a filled rectangle with rounded corners
    // Example: display.rectRounded(10, 10, 100, 50, 5, GREEN);
    //   radius = corner radius in pixels
    void rectRounded(int x, int y, int width, int height, int radius, uint16_t color = WHITE);

    // Draw a circle outline
    // Example: display.circle(160, 86, 40, YELLOW);
    //   cx, cy = centre point;  radius = radius in pixels
    void circle(int cx, int cy, int radius, uint16_t color = WHITE);

    // Draw a filled circle
    void circleFilled(int cx, int cy, int radius, uint16_t color = WHITE);

    // Draw a line between two points
    // Example: display.line(0, 0, 319, 171, RED);
    void line(int x1, int y1, int x2, int y2, uint16_t color = WHITE);

    // Draw a 16-bit (RGB565) image, e.g. one generated from a photo or icon.
    // Example: display.drawBitmap(10, 10, myImage, 16, 16);
    //   bitmap        = array of pixel colors, row by row, top-left to bottom-right
    //   width, height = size of the image in pixels (must match the array!)
    // See examples/04_bitmap_image.cpp for how to create your own image array.
    void drawBitmap(int x, int y, const uint16_t* bitmap, int width, int height);

    // Draw a progress bar for a sensor value (e.g. temperature, brightness)
    // Example: display.bar(10, 80, 200, 20, value, 0, 100, CYAN);
    //   x, y          = position
    //   width, height = size of the bar
    //   value         = current value
    //   minValue      = lower end of the scale
    //   maxValue      = upper end of the scale
    //   color         = bar colour
    void bar(int x, int y, int width, int height,
             float value, float minValue, float maxValue,
             uint16_t color = GREEN);

    // Direct access to the underlying TFT object (advanced use)
#ifdef USE_ARDUINO_GFX
    Arduino_GFX& tft() { return *_gfx; }
#else
    TFT_eSPI& tft() { return _tft; }
#endif

private:
#ifdef USE_ARDUINO_GFX
    Arduino_DataBus* _bus = nullptr;
    Arduino_GFX* _gfx = nullptr;
#else
    TFT_eSPI _tft;
#endif
};
