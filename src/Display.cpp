#include "Display.h"

void Display::begin() {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
    _tft.init();
    _tft.setRotation(3);    // 0 = Portrait  172 × 320 px
                            // 1 = Landscape 320 × 172 px
                            // 2 = Portrait gedreht  172 × 320 px
                            // 3 = Landscape gedreht 320 × 172 px
    _tft.fillScreen(TFT_BLACK);
}

void Display::clear(uint16_t color) {
    _tft.fillScreen(color);
}

void Display::print(const String& text, int x, int y,
                    uint16_t color, uint8_t size) {
    _tft.setTextColor(color, _tft.color565(0, 0, 0));
    _tft.setTextSize(size);
    _tft.setTextFont(0);    // built-in GLCD font, always available
    _tft.drawString(text, x, y);
}

void Display::number(float value, int x, int y,
                     uint16_t color, uint8_t size,
                     uint8_t decimals) {
    char buf[16];
    dtostrf(value, 1, decimals, buf);
    print(String(buf), x, y, color, size);
}

void Display::rect(int x, int y, int width, int height, uint16_t color) {
    _tft.drawRect(x, y, width, height, color);
}

void Display::rectFilled(int x, int y, int width, int height, uint16_t color) {
    _tft.fillRect(x, y, width, height, color);
}

void Display::circle(int cx, int cy, int radius, uint16_t color) {
    _tft.drawCircle(cx, cy, radius, color);
}

void Display::circleFilled(int cx, int cy, int radius, uint16_t color) {
    _tft.fillCircle(cx, cy, radius, color);
}

void Display::line(int x1, int y1, int x2, int y2, uint16_t color) {
    _tft.drawLine(x1, y1, x2, y2, color);
}

void Display::bar(int x, int y, int width, int height,
                  float value, float minValue, float maxValue,
                  uint16_t color) {
    // Draw the empty bar outline
    _tft.drawRect(x, y, width, height, GRAY);

    // Map value to bar width
    float ratio = constrain((value - minValue) / (maxValue - minValue), 0.0f, 1.0f);
    int filled = (int)(ratio * (width - 2));

    // Draw the filled portion
    if (filled > 0) {
        _tft.fillRect(x + 1, y + 1, filled, height - 2, color);
    }
    // Clear the empty portion (needed when the value decreases)
    if (filled < width - 2) {
        _tft.fillRect(x + 1 + filled, y + 1,
                      width - 2 - filled, height - 2, BLACK);
    }
}
