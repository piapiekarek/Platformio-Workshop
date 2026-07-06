#pragma once

#include <Arduino.h>

class ServoSweep {
public:
    void begin();
    void update();

private:
    static const uint16_t MIN_US = 500;
    static const uint16_t MAX_US = 2400;

    int _angle = 0;
    int _step = 2;
};
