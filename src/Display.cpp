#include "Display.h"

#ifdef USE_ARDUINO_GFX
// Init sequence mirrored from Waveshare RP2350-LCD-1.47 wiki demo.
static const uint8_t kWaveshareSt7789Init[] = {
    BEGIN_WRITE,
    WRITE_C8_D8, 0x36, 0x70,
    WRITE_COMMAND_8, 0x11,
    END_WRITE,
    DELAY, 120,

    BEGIN_WRITE,
    WRITE_C8_D8, 0x36, 0x70,
    WRITE_C8_D8, 0x3A, 0x05,
    WRITE_C8_BYTES, 0xB2, 5, 0x0C, 0x0C, 0x00, 0x33, 0x33,
    WRITE_C8_D8, 0xB7, 0x35,
    WRITE_C8_D8, 0xBB, 0x35,
    WRITE_C8_D8, 0xC0, 0x2C,
    WRITE_C8_D8, 0xC2, 0x01,
    WRITE_C8_D8, 0xC3, 0x13,
    WRITE_C8_D8, 0xC4, 0x20,
    WRITE_C8_D8, 0xC6, 0x0F,
    WRITE_C8_D16, 0xD0, 0xA4, 0xA1,
    WRITE_C8_D8, 0xD6, 0xA1,
    WRITE_C8_BYTES, 0xE0, 14,
        0xF0, 0x00, 0x04, 0x04, 0x04, 0x05, 0x29, 0x33,
        0x3E, 0x38, 0x12, 0x12, 0x28, 0x30,
    WRITE_C8_BYTES, 0xE1, 14,
        0xF0, 0x07, 0x0A, 0x0D, 0x0B, 0x07, 0x28, 0x33,
        0x3E, 0x36, 0x14, 0x14, 0x29, 0x32,
    WRITE_COMMAND_8, 0x21,
    WRITE_COMMAND_8, 0x11,
    END_WRITE,
    DELAY, 120,

    BEGIN_WRITE,
    WRITE_COMMAND_8, 0x29,
    END_WRITE
};
#endif

void Display::begin() {
#ifdef USE_ARDUINO_GFX
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    pinMode(TFT_BL, OUTPUT);

    digitalWrite(TFT_CS, HIGH);
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);

    // Waveshare wiki-style reset pulse
    digitalWrite(TFT_RST, HIGH);
    delay(100);
    digitalWrite(TFT_RST, LOW);
    delay(100);
    digitalWrite(TFT_RST, HIGH);
    delay(100);

    #if defined(TFT_MISO) && (TFT_MISO >= 0)
    SPI.setRX(TFT_MISO);
    #endif
    SPI.setCS(TFT_CS);
    SPI.setTX(TFT_MOSI);
    SPI.setSCK(TFT_SCLK);
    SPI.begin();
    SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

    _bus = new Arduino_HWSPI(TFT_DC, TFT_CS, &SPI, false);
    _gfx = new Arduino_ST7789(
        _bus,
        TFT_RST,
        0,
        true,
        172,
        320,
        34,
        0,
        34,
        0,
        kWaveshareSt7789Init,
        sizeof(kWaveshareSt7789Init));

    if (!_gfx->begin(40000000)) {
        return;
    }
    _gfx->fillScreen(BLACK);
#else
#if defined(TFT_RST) && (TFT_RST >= 0)
    // Force a clean LCD controller reset before init sequence.
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, HIGH);
    delay(5);
    digitalWrite(TFT_RST, LOW);
    delay(20);
    digitalWrite(TFT_RST, HIGH);
    delay(150);
#endif

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);

    _tft.init();
    _tft.setRotation(3);    // 0 = Portrait  172 × 320 px
                            // 1 = Landscape 320 × 172 px
                            // 2 = Portrait gedreht  172 × 320 px
                            // 3 = Landscape gedreht 320 × 172 px

    // Startup self-test: if these colors are visible, SPI + init works.
    _tft.fillScreen(TFT_RED);
    delay(150);
    _tft.fillScreen(TFT_GREEN);
    delay(150);
    _tft.fillScreen(TFT_BLUE);
    delay(150);
    _tft.fillScreen(TFT_BLACK);
#endif
}

void Display::setRotation(uint8_t rotation) {
#ifdef USE_ARDUINO_GFX
    if (_gfx) {
        _gfx->setRotation(rotation & 0x03);
    }
#else
    _tft.setRotation(rotation & 0x03);
#endif
}

void Display::clear(uint16_t color) {
#ifdef USE_ARDUINO_GFX
    if (_gfx) {
        _gfx->fillScreen(color);
    }
#else
    _tft.fillScreen(color);
#endif
}

void Display::print(const String& text, int x, int y,
                    uint16_t color, uint8_t size) {
#ifdef USE_ARDUINO_GFX
    if (!_gfx) {
        return;
    }
    _gfx->setTextColor(color);
    _gfx->setTextSize(size);
    _gfx->setCursor(x, y);
    _gfx->print(text);
#else
    _tft.setTextColor(color, _tft.color565(0, 0, 0));
    _tft.setTextSize(size);
    _tft.setTextFont(0);    // built-in GLCD font, always available
    _tft.drawString(text, x, y);
#endif
}

void Display::number(float value, int x, int y,
                     uint16_t color, uint8_t size,
                     uint8_t decimals) {
    print(String(value, decimals), x, y, color, size);
}

void Display::rect(int x, int y, int width, int height, uint16_t color) {
#ifdef USE_ARDUINO_GFX
    if (_gfx) {
        _gfx->drawRect(x, y, width, height, color);
    }
#else
    _tft.drawRect(x, y, width, height, color);
#endif
}

void Display::rectFilled(int x, int y, int width, int height, uint16_t color) {
#ifdef USE_ARDUINO_GFX
    if (_gfx) {
        _gfx->fillRect(x, y, width, height, color);
    }
#else
    _tft.fillRect(x, y, width, height, color);
#endif
}

void Display::rectRounded(int x, int y, int width, int height, int radius, uint16_t color) {
    // Clamp radius to reasonable bounds
    radius = constrain(radius, 1, (width < height ? width : height) / 2);

    // Draw filled rectangles for the main body and sides
    rectFilled(x + radius, y, width - 2 * radius, height, color);
    rectFilled(x, y + radius, width, height - 2 * radius, color);

    // Draw four corners as filled circles
    circleFilled(x + radius, y + radius, radius, color);              // top-left
    circleFilled(x + width - radius - 1, y + radius, radius, color);  // top-right
    circleFilled(x + radius, y + height - radius - 1, radius, color); // bottom-left
    circleFilled(x + width - radius - 1, y + height - radius - 1, radius, color); // bottom-right
}

void Display::circle(int cx, int cy, int radius, uint16_t color) {
#ifdef USE_ARDUINO_GFX
    if (_gfx) {
        _gfx->drawCircle(cx, cy, radius, color);
    }
#else
    _tft.drawCircle(cx, cy, radius, color);
#endif
}

void Display::circleFilled(int cx, int cy, int radius, uint16_t color) {
#ifdef USE_ARDUINO_GFX
    if (_gfx) {
        _gfx->fillCircle(cx, cy, radius, color);
    }
#else
    _tft.fillCircle(cx, cy, radius, color);
#endif
}

void Display::line(int x1, int y1, int x2, int y2, uint16_t color) {
#ifdef USE_ARDUINO_GFX
    if (_gfx) {
        _gfx->drawLine(x1, y1, x2, y2, color);
    }
#else
    _tft.drawLine(x1, y1, x2, y2, color);
#endif
}

void Display::drawBitmap(int x, int y, const uint16_t* bitmap, int width, int height) {
#ifdef USE_ARDUINO_GFX
    if (_gfx) {
        _gfx->draw16bitRGBBitmap(x, y, bitmap, width, height);
    }
#else
    _tft.pushImage(x, y, width, height, bitmap);
#endif
}

void Display::bar(int x, int y, int width, int height,
                  float value, float minValue, float maxValue,
                  uint16_t color) {
#ifdef USE_ARDUINO_GFX
    if (!_gfx) {
        return;
    }
    _gfx->drawRect(x, y, width, height, GRAY);

    float ratio = constrain((value - minValue) / (maxValue - minValue), 0.0f, 1.0f);
    int filled = (int)(ratio * (width - 2));

    if (filled > 0) {
        _gfx->fillRect(x + 1, y + 1, filled, height - 2, color);
    }
    if (filled < width - 2) {
        _gfx->fillRect(x + 1 + filled, y + 1,
                       width - 2 - filled, height - 2, BLACK);
    }
#else
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
#endif
}
