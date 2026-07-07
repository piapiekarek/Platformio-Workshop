// =============================================================================
//  RP2350 Simple Project: Button + LED + Temperature Sensor
// =============================================================================
//
// This code is intentionally simple and linear.
// No class nonsense – just Arduino APIs everyone understands!
//
// =============================================================================

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Display.h"

// ────────────────────────────────────────────────────────────────────────────
// PIN DEFINITIONS
// ────────────────────────────────────────────────────────────────────────────
// Only the pins we actually use – easy to understand!

// Pins (from pin.h, repeated here for reference)
const int BUTTON_PIN = 0;      // Button on GPIO 0
const int LED_PIN = 4;         // LED on GPIO 4
const int TEMP_SENSOR_PIN = 3; // Temperature sensor on GPIO 3

// Temperature calibration offset (in °C)
// Adjust this value if your sensor reads too high or too low
// Example: If sensor reads 28.6°C but actual is 26.3°C, offset = -2.3
const float TEMP_CALIBRATION_OFFSET = -2.3;  // Current calibration

// ────────────────────────────────────────────────────────────────────────────
// GLOBAL VARIABLES (Application state)
// ────────────────────────────────────────────────────────────────────────────

Display display;               // The display

bool ledIsOn = false;          // Is the LED on or off?
float temperature = 0.0;       // Current temperature
bool tempSensorConnected = false; // Is the sensor connected?

const unsigned long TEMP_READ_INTERVAL_MS = 1500;
const unsigned long TEMP_CONVERSION_MS = 750;

unsigned long lastTempReadTime = 0;
unsigned long tempRequestStartTime = 0;
bool tempRequestInProgress = false;

// Global sensor objects
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

// ────────────────────────────────────────────────────────────────────────────
// SETUP FUNCTIONS
// ────────────────────────────────────────────────────────────────────────────

void setupDisplay() {
    display.begin();
    display.setRotation(3);
    display.clear(BLACK);
    
    // Startup screen
    display.print("Starting...", 50, 80, WHITE, 2);
}


void setupButton() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);// DO NOT CHANGE THIS!
    // INPUT_PULLUP = Button connected to GND, pulls pin to HIGH
}

void setupLed() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // LED off
    ledIsOn = false;
}

void setupTempSensor() {
    tempSensor.begin();
    tempSensor.setWaitForConversion(false);
}

// ────────────────────────────────────────────────────────────────────────────
// DISPLAY UPDATE FUNCTIONS
// ────────────────────────────────────────────────────────────────────────────

void updateDisplay() {
    // Clear screen
    display.rectFilled(0, 0, 320, 172, WHITE);
    
    // TITLE
    display.print("Control Panel", 10, 6, BLACK, 2);
    display.line(0, 24, 319, 24, GRAY);
    
    // SENSOR STATUS
    display.rect(6, 30, 308, 26, GRAY);
    display.print("Sensor: ", 12, 34, BLACK, 1);
    if (tempSensorConnected) {
        display.print("CONNECTED", 72, 34, GREEN, 1);
    } else {
        display.print("UNCONNECTED", 72, 34, RED, 1);
    }
    
    // BUTTON STATUS
    display.rect(6, 60, 150, 26, GRAY);
    display.print("Button: ", 12, 64, CYAN, 1);
    if (digitalRead(BUTTON_PIN) == LOW) {
        display.print("PRESSED", 84, 64, MAGENTA, 1);
    } else {
        display.print("released", 84, 64, BLACK, 1);
    }
    
    // LED STATUS
    display.rect(162, 60, 152, 26, GRAY);
    display.print("LED: ", 168, 64, BLACK, 1);
    if (ledIsOn) {
        display.print("ON", 204, 64, GREEN, 1);
    } else {
        display.print("OFF", 204, 64, GRAY, 1);
    }
    
    // Note: This function is called after oneWire is defined globally above
    
    // TEMPERATURE
    display.rect(6, 90, 308, 74, GRAY);
    display.print("Temperature:", 12, 96, CYAN, 1);
    
    // Large temperature display
    display.number(temperature, 16, 114, MAGENTA, 3, 1);
    display.print("C", 88, 114, MAGENTA, 3);
    
    // Info text
    display.print("Connect sensor or press button!", 8, 155, ORANGE, 1);
}

// ────────────────────────────────────────────────────────────────────────────
// SENSOR & INPUT FUNCTIONS
// ────────────────────────────────────────────────────────────────────────────

void readTemperature() {
    unsigned long now = millis();

    // Start a new conversion every interval.
    if (!tempRequestInProgress && (now - lastTempReadTime >= TEMP_READ_INTERVAL_MS)) {
        tempSensor.requestTemperatures();
        tempRequestStartTime = now;
        tempRequestInProgress = true;
        return;
    }

    // Wait until conversion is done without blocking the loop.
    if (!tempRequestInProgress || (now - tempRequestStartTime < TEMP_CONVERSION_MS)) {
        return;
    }

    lastTempReadTime = now;
    tempRequestInProgress = false;

    // Get value
    float t = tempSensor.getTempCByIndex(0);
    
    // Check if sensor is connected
    if (t == DEVICE_DISCONNECTED_C) {
        tempSensorConnected = false;
        temperature = 0.0;
    } else {
        tempSensorConnected = true;
        // Apply calibration offset
        temperature = t + TEMP_CALIBRATION_OFFSET;
    }
}

void handleButton() {
    int currentButtonState = digitalRead(BUTTON_PIN);

    // Simple behaviour for workshop:
    // Button pressed (LOW) -> LED ON, button released (HIGH) -> LED OFF.
    ledIsOn = (currentButtonState == LOW);
    digitalWrite(LED_PIN, ledIsOn ? HIGH : LOW);
    
}

// ────────────────────────────────────────────────────────────────────────────
// MAIN FUNCTIONS
// ────────────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("Setup starting...");
    
    setupDisplay();
    setupButton();
    setupLed();
    setupTempSensor();
    
    Serial.println("Setup complete!");
    delay(1000);
}

void loop() {
    // Store old values to avoid flickering
    static float lastTempDisplay = -999;
    static bool lastLedDisplay = false;
    static bool lastSensorDisplay = false;
    static bool lastButtonDisplay = false;
    
    bool displayNeedsUpdate = false;
    
    // 1. Read temperature (if enough time has passed)
    readTemperature();
    if (temperature != lastTempDisplay || tempSensorConnected != lastSensorDisplay) {
        displayNeedsUpdate = true;
        lastTempDisplay = temperature;
        lastSensorDisplay = tempSensorConnected;
    }
    
    // 2. Check button
    handleButton();
    
    bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
    if (ledIsOn != lastLedDisplay || buttonPressed != lastButtonDisplay) {
        displayNeedsUpdate = true;
        lastLedDisplay = ledIsOn;
        lastButtonDisplay = buttonPressed;
    }
    
    // 3. Only update display if something changed
    if (displayNeedsUpdate) {
        updateDisplay();
    }
    
    // Small delay (don't waste CPU)
    delay(50);
}
