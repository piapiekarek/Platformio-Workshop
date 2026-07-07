#pragma once

#include <Arduino.h>

class TemperatureSensor {
public:
    void begin();
    bool updateIfDue(uint32_t nowMs);

    bool isConnected() const { return _connected; }
    float celsius() const { return _celsius; }

private:
    static const uint32_t UPDATE_INTERVAL_MS = 1000;

    uint32_t _lastReadMs = 0;
    bool _connected = true;
    float _celsius = 0.0f;
};
