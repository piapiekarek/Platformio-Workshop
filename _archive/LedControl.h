#pragma once

#include <Arduino.h>

class LedControl {
public:
    void begin();

    void toggleSimpleLed();
    bool isSimpleLedOn() const { return _simpleLedOn; }

    void cycleRgbColor();
    const char* rgbColorName() const;

private:
    enum class RgbColor : uint8_t {
        Off = 0,
        Red = 1,
        Green = 2,
        Blue = 3,
    };

    void applyRgbColor();

    bool _simpleLedOn = false;
    RgbColor _rgbColor = RgbColor::Off;
};
