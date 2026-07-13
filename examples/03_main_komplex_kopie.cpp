// =============================================================================
//  RP2350 Control Panel: button + LED + temperature sensor + RFID reader
// =============================================================================
//
//  What does this code do?
//  - A button toggles an LED on/off (short click).
//  - A DS18B20 temperature sensor is read regularly.
//  - An RFID reader (RC522) detects cards and shows their ID.
//    (only if RFID_ENABLED is set to 1 in pin.h, see src/Rfid.cpp)
//  - All info is shown on the display.
//
//  Pin numbers live in pin.h. The display pins are fixed on the board and
//  therefore live in platformio.ini (build_flags) instead.
// =============================================================================

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Display.h"
#include "pin.h"
#include "SpiBus.h"
#include "Rfid.h"
#include "RgbLed.h"

// ────────────────────────────────────────────────────────────────────────────
// SETTINGS
// ────────────────────────────────────────────────────────────────────────────

// Temperature sensor calibration offset in °C.
// Example: sensor reads 28.6°C, actual temperature is 26.3°C -> offset = -2.3
const float TEMP_CALIBRATION_OFFSET = -2.3;

const unsigned long TEMP_READ_INTERVAL_MS = 1500; // how often the temperature is read
const unsigned long BUTTON_LONG_PRESS_MS = 1000;   // when a click counts as "long"

// ────────────────────────────────────────────────────────────────────────────
// GLOBAL VARIABLES (current program state)
// ────────────────────────────────────────────────────────────────────────────

Display display;

bool ledIsOn = false;
float temperature = 0.0;
bool tempSensorConnected = false;

bool lastButtonState = HIGH; // HIGH = not pressed (because of INPUT_PULLUP)
unsigned long buttonPressTime = 0;

unsigned long lastTempReadTime = 0;

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

// ────────────────────────────────────────────────────────────────────────────
// SETUP FUNCTIONS
// ────────────────────────────────────────────────────────────────────────────

void setupDisplay() {
    display.begin();
    display.setRotation(3);
    display.clear(BLACK);
    display.print("Starting...", 50, 78, WHITE, 2);
}

void setupButton() {
    pinMode(BUTTON_PIN, INPUT_PULLUP); // button is wired to GND, resting state = HIGH
}

void setupLed() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    ledIsOn = false;
}

void setupTempSensor() {
    tempSensor.begin();
}

// ────────────────────────────────────────────────────────────────────────────
// DISPLAY DRAWING
// ────────────────────────────────────────────────────────────────────────────

void updateDisplay() {
    display.rectFilled(0, 0, 320, 172, WHITE);

    // TITLE
    display.print("Control Panel", 10, 4, BLACK, 2);
    display.line(0, 24, 319, 24, GRAY);

    // SENSOR STATUS
    display.rect(6, 30, 308, 26, GRAY);
    display.print("Sensor: ", 12, 39, BLACK, 1);
    if (tempSensorConnected) {
        display.print("CONNECTED", 72, 39, DARKGREEN, 1);
    } else {
        display.print("UNCONNECTED", 72, 39, DARKRED, 1);
    }

    // BUTTON STATUS
    display.rect(6, 60, 150, 26, GRAY);
    display.print("Button: ", 12, 69, DARKCYAN, 1);
    if (digitalRead(BUTTON_PIN) == LOW) {
        display.print("PRESSED", 84, 69, DARKMAGENTA, 1);
    } else {
        display.print("released", 84, 69, BLACK, 1);
    }

    // LED STATUS
    display.rect(162, 60, 152, 26, GRAY);
    display.print("LED: ", 168, 69, BLACK, 1);
    if (ledIsOn) {
        display.print("ON", 204, 69, DARKGREEN, 1);
    } else {
        display.print("OFF", 204, 69, DARKGRAY, 1);
    }

    // TEMPERATURE - label and value in one line, same text size (so they
    // visually belong together instead of the value floating below).
    display.rect(6, 90, 308, 30, GRAY);
    display.print("Temperature:", 12, 97, DARKCYAN, 2);
    display.print(String(temperature, 1) + " C", 170, 97, DARKMAGENTA, 2);

    // RFID STATUS
    display.rect(6, 124, 308, 26, GRAY);
    display.print("RFID:", 12, 133, BLACK, 1);

    uint16_t rfidStatusColor = DARKGRAY;
    if (!RFID_ENABLED) {
        display.print("disabled", 52, 133, DARKGRAY, 1);
    } else if (!rfidIsAvailable()) {
        display.print("reader missing", 52, 133, DARKRED, 1);
        rfidStatusColor = DARKRED;
    } else {
        display.print("ready", 52, 133, DARKGREEN, 1);
        rfidStatusColor = DARKGREEN;
    }

    // Show the card ID to the right of the status dot
    String cardId = rfidLastCardId();
    if (cardId.length() > 0) {
        display.print(cardId, 210, 133, BLUE, 1);
        rfidStatusColor = BLUE;
    }
    display.circleFilled(300, 137, 6, rfidStatusColor); // gray=off, red=missing, green=ready, blue=card read

    display.print("Press button or scan RFID card!", 8, 158, DARKORANGE, 1);
}

// ────────────────────────────────────────────────────────────────────────────
// SENSORS & INPUT
// ────────────────────────────────────────────────────────────────────────────

void readTemperature() {
    unsigned long now = millis();
    if (now - lastTempReadTime < TEMP_READ_INTERVAL_MS) {
        return;
    }
    lastTempReadTime = now;

    tempSensor.requestTemperatures();
    delay(750); // DS18B20 needs at least 750ms for a 12-bit reading

    float t = tempSensor.getTempCByIndex(0);
    if (t == DEVICE_DISCONNECTED_C) {
        tempSensorConnected = false;
        temperature = 0.0;
    } else {
        tempSensorConnected = true;
        temperature = t + TEMP_CALIBRATION_OFFSET;
    }
}

void handleButton() {
    int currentButtonState = digitalRead(BUTTON_PIN);

    // LED follows the button directly: on while held down, off when released.
    bool newLedState = (currentButtonState == LOW);
    if (newLedState != ledIsOn) {
        ledIsOn = newLedState;
        digitalWrite(LED_PIN, ledIsOn ? HIGH : LOW);
        Serial.print("LED is now: ");
        Serial.println(ledIsOn ? "ON" : "OFF");
    }

    if (currentButtonState == LOW && lastButtonState == HIGH) {
        buttonPressTime = millis(); // button was just pressed
    }

    if (currentButtonState == HIGH && lastButtonState == LOW) {
        unsigned long pressDuration = millis() - buttonPressTime; // button was released

        if (pressDuration >= BUTTON_LONG_PRESS_MS) {
            // Long press: add your own action here, e.g.:
            // rgbLedFill(255, 0, 0); rgbLedShow();
        }
    }

    lastButtonState = currentButtonState;
}

// ────────────────────────────────────────────────────────────────────────────
// SETUP & LOOP
// ────────────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Setup starting...");

    setupDisplay();
    setupButton();
    setupLed();
    setupTempSensor();
    rfidSetup();
    rgbLedSetup();

    // Draw the full dashboard once right away instead of staying on "Starting..."
    selectDisplayBus();
    updateDisplay();

    Serial.println("Setup complete!");
    delay(1000);
}

void loop() {
    // Remember the last shown values so the display is only redrawn on real
    // changes (no flicker).
    static float lastTempDisplay = -999;
    static bool lastLedDisplay = false;
    static bool lastSensorDisplay = false;
    static bool lastButtonDisplay = false;
    static String lastCardDisplay = "";
    static bool lastRfidAvailableState = false;

    bool displayNeedsUpdate = false;

    // 1. Read temperature
    readTemperature();
    if (temperature != lastTempDisplay || tempSensorConnected != lastSensorDisplay) {
        displayNeedsUpdate = true;
        lastTempDisplay = temperature;
        lastSensorDisplay = tempSensorConnected;
    }

    // 2. Check button
    handleButton();

    // 3. Update RFID in the background (init attempt + card polling)
    rfidUpdate();
    if (rfidIsAvailable() != lastRfidAvailableState || rfidLastCardId() != lastCardDisplay) {
        displayNeedsUpdate = true;
        lastRfidAvailableState = rfidIsAvailable();
        lastCardDisplay = rfidLastCardId();
    }

    bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
    if (ledIsOn != lastLedDisplay || buttonPressed != lastButtonDisplay) {
        displayNeedsUpdate = true;
        lastLedDisplay = ledIsOn;
        lastButtonDisplay = buttonPressed;
    }

    // 4. Only redraw the display if something actually changed
    if (displayNeedsUpdate) {
        selectDisplayBus();
        updateDisplay();
    }

    delay(50); // don't hog the CPU
}
