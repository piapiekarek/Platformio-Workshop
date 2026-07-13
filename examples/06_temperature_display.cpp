// =============================================================================
//  Example: temperature sensor + display
// =============================================================================
//
//  Reads a DS18B20 temperature sensor and shows the value on the display.
// =============================================================================

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Display.h"
#include "pin.h"

Display display;
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

const unsigned long READ_INTERVAL_MS = 1500; // how often the temperature is read
unsigned long lastReadTime = 0;

void setup() {
    display.begin();
    display.setRotation(3);
    display.clear(WHITE);
    tempSensor.begin();
}

void loop() {
    unsigned long now = millis();
    if (now - lastReadTime < READ_INTERVAL_MS) {
        return;
    }
    lastReadTime = now;

    tempSensor.requestTemperatures();
    delay(750); // DS18B20 needs at least 750ms for a 12-bit reading
    float t = tempSensor.getTempCByIndex(0);

    display.rectFilled(0, 0, 320, 172, WHITE);
    display.print("Temperature", 20, 20, DARKCYAN, 2);

    if (t == DEVICE_DISCONNECTED_C) {
        display.print("Sensor not connected!", 20, 70, DARKRED, 2);
    } else {
        display.print(String(t, 1) + " C", 40, 70, DARKMAGENTA, 4);
    }
}
