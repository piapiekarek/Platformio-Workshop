#include "TemperatureSensor.h"

#include <DallasTemperature.h>
#include <OneWire.h>

#include "pin.h"

static OneWire g_oneWire(PIN_DS18B20);
static DallasTemperature g_ds18b20(&g_oneWire);

void TemperatureSensor::begin() {
    g_ds18b20.begin();
}

bool TemperatureSensor::updateIfDue(uint32_t nowMs) {
    if (nowMs - _lastReadMs < UPDATE_INTERVAL_MS) {
        return false;
    }

    _lastReadMs = nowMs;
    g_ds18b20.requestTemperatures();

    float t = g_ds18b20.getTempCByIndex(0);
    if (t == DEVICE_DISCONNECTED_C) {
        _connected = false;
        return true;
    }

    _connected = true;
    _celsius = t;
    return true;
}
