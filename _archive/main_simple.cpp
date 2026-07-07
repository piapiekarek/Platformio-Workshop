// =============================================================================
//  RP2350 Einfach-Projekt: Button + LED + Temperatur-Sensor
// =============================================================================
//
// Dieser Code ist absichtlich einfach und linear geschrieben.
// Kein Klassen-Gelaber – nur Arduino-APIs, die jeder versteht!
//
// =============================================================================

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Display.h"

// ────────────────────────────────────────────────────────────────────────────
// PIN-DEFINITIONEN
// ────────────────────────────────────────────────────────────────────────────
// Nur die Pins die wir wirklich nutzen – einfach zu verstehen!

// Pins (aus pin.h, hier wiederholt für Überblick)
const int BUTTON_PIN = 0;      // Button an GPIO 0
const int LED_PIN = 4;         // LED an GPIO 4
const int TEMP_SENSOR_PIN = 3; // Temp-Sensor an GPIO 3

// ────────────────────────────────────────────────────────────────────────────
// GLOBALE VARIABLEN (Der Zustand unserer Anwendung)
// ────────────────────────────────────────────────────────────────────────────

Display display;               // Das Display

bool ledIsOn = false;          // Ist die LED an oder aus?
float temperature = 0.0;       // Aktuelle Temperatur
bool tempSensorConnected = false; // Ist der Sensor angeschlossen?

// Für den Button
bool lastButtonState = HIGH;   // HIGH = nicht gedrückt, LOW = gedrückt
unsigned long buttonPressTime = 0;

// Für den Temperatur-Sensor
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);
unsigned long lastTempReadTime = 0;

// ────────────────────────────────────────────────────────────────────────────
// FUNKTIONEN - SETUP
// ────────────────────────────────────────────────────────────────────────────

void setupDisplay() {
    display.begin();
    display.setRotation(3);
    display.clear(BLACK);
    
    // Startbildschirm
    display.print("Starten...", 50, 80, WHITE, 2);
}

void setupButton() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    // INPUT_PULLUP = Button verbunden zu GND, zieht Pin auf HIGH
}

void setupLed() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW); // LED aus
    ledIsOn = false;
}

void setupTempSensor() {
    tempSensor.begin();
}

// ────────────────────────────────────────────────────────────────────────────
// FUNKTIONEN - DISPLAY-AKTUALISIERUNG
// ────────────────────────────────────────────────────────────────────────────

void updateDisplay() {
    // Bildschirm löschen
    display.rectFilled(0, 0, 320, 172, BLACK);
    
    // TITEL
    display.print("Control Panel", 10, 6, WHITE, 2);
    display.line(0, 24, 319, 24, GRAY);
    
    // SENSOR STATUS
    display.rect(6, 30, 308, 26, GRAY);
    display.print("Sensor: ", 12, 34, WHITE, 1);
    if (tempSensorConnected) {
        display.print("CONNECTED", 72, 34, GREEN, 1);
    } else {
        display.print("UNCONNECTED", 72, 34, RED, 1);
    }
    
    // BUTTON STATUS
    display.rect(6, 60, 150, 26, GRAY);
    display.print("Button: ", 12, 64, CYAN, 1);
    if (digitalRead(BUTTON_PIN) == LOW) {
        display.print("PRESSED", 84, 64, YELLOW, 1);
    } else {
        display.print("released", 84, 64, WHITE, 1);
    }
    
    // LED STATUS
    display.rect(162, 60, 152, 26, GRAY);
    display.print("LED: ", 168, 64, WHITE, 1);
    if (ledIsOn) {
        display.print("ON", 204, 64, GREEN, 1);
    } else {
        display.print("OFF", 204, 64, GRAY, 1);
    }
    
    // TEMPERATUR
    display.rect(6, 90, 308, 74, GRAY);
    display.print("Temperatur:", 12, 96, CYAN, 1);
    
    // Große Temperatur-Anzeige
    display.number(temperature, 16, 114, YELLOW, 3, 1);
    display.print("C", 78, 114, YELLOW, 3);
    
    // Info-Text
    display.print("Schliess Sensor an oder druecke Button!", 8, 155, ORANGE, 1);
}

// ────────────────────────────────────────────────────────────────────────────
// FUNKTIONEN - SENSOR & INPUT
// ────────────────────────────────────────────────────────────────────────────

void readTemperature() {
    unsigned long now = millis();
    
    // Nur alle 1 Sekunde lesen (nicht ständig!)
    if (now - lastTempReadTime < 1000) {
        return;
    }
    
    lastTempReadTime = now;
    
    // Neuen Wert anfordern
    tempSensor.requestTemperatures();
    
    // Wert abholen
    float t = tempSensor.getTempCByIndex(0);
    
    // Prüfen ob Sensor angeschlossen ist
    if (t == DEVICE_DISCONNECTED_C) {
        tempSensorConnected = false;
        temperature = 0.0;
    } else {
        tempSensorConnected = true;
        temperature = t;
    }
}

void handleButton() {
    int currentButtonState = digitalRead(BUTTON_PIN);
    
    // Wenn Button gerade gedrückt wurde (von HIGH zu LOW)
    if (currentButtonState == LOW && lastButtonState == HIGH) {
        buttonPressTime = millis();
    }
    
    // Wenn Button losgelassen wurde (von LOW zu HIGH)
    if (currentButtonState == HIGH && lastButtonState == LOW) {
        unsigned long pressDuration = millis() - buttonPressTime;
        
        // Kurz geklickt (< 1 Sekunde)
        if (pressDuration < 1000) {
            // LED umschalten
            ledIsOn = !ledIsOn;
            if (ledIsOn) {
                digitalWrite(LED_PIN, HIGH);
            } else {
                digitalWrite(LED_PIN, LOW);
            }
        }
        // Lang geklickt (> 1 Sekunde)
        else {
            // Beide LEDs blinken? Oder andere Aktion...
            // Für jetzt: noch nichts
        }
    }
    
    lastButtonState = currentButtonState;
}

// ────────────────────────────────────────────────────────────────────────────
// MAIN FUNKTIONEN
// ────────────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("Setup startet...");
    
    setupDisplay();
    setupButton();
    setupLed();
    setupTempSensor();
    
    Serial.println("Setup fertig!");
    delay(1000);
}

void loop() {
    // 1. Temperatur einlesen (wenn genug Zeit vergangen)
    readTemperature();
    
    // 2. Button prüfen
    handleButton();
    
    // 3. Display aktualisieren
    updateDisplay();
    
    // Kleine Pause (nicht zu viel CPU verschwenden)
    delay(50);
}
