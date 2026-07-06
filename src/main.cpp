#include <Arduino.h>
#include "Display.h"
#include "AppScreen.h"
#include "ButtonInput.h"
#include "LedControl.h"
#include "ServoSweep.h"
#include "TemperatureSensor.h"

// ═══════════════════════════════════════════════════════════════════════════
//  Waveshare 1.47" LCD – starter project
// ═══════════════════════════════════════════════════════════════════════════
//
//  The display is 320 pixels wide and 172 pixels tall (landscape).
//
//        (0,0) ──────────────────────── (319,0)
//          │                                │
//          │                                │
//        (0,171) ────────────────── (319,171)
//
//  Colours: BLACK  WHITE  RED  GREEN  BLUE
//           YELLOW  CYAN  MAGENTA  GRAY  ORANGE
//
// ═══════════════════════════════════════════════════════════════════════════

Display display;
AppScreen screen(display);
ButtonInput buttons;
LedControl leds;
ServoSweep servo;
TemperatureSensor sensor;

enum class UiMode : uint8_t {
    Dashboard = 0,
    Menu,
};

UiMode uiMode = UiMode::Dashboard;
uint8_t menuIndex = 0;
bool dashboardDirty = true;
bool prevB1 = false;
bool prevB2 = false;
bool prevB3 = false;

DashboardData makeDashboardData() {
    DashboardData data;
    data.sensorConnected = sensor.isConnected();
    data.ledOn = leds.isSimpleLedOn();
    data.rgbName = leds.rgbColorName();
    data.button1Pressed = buttons.isPressed(0);
    data.button2Pressed = buttons.isPressed(1);
    data.button3Pressed = buttons.isPressed(2);
    data.temperatureC = sensor.celsius();
    return data;
}

void drawDashboard() {
    screen.drawDashboard(makeDashboardData());
    dashboardDirty = false;
}

void runMenuAction() {
    switch (menuIndex % 3) {
        case 0:
            leds.toggleSimpleLed();
            break;
        case 1:
            leds.cycleRgbColor();
            break;
        default:
            uiMode = UiMode::Dashboard;
            drawDashboard();
            return;
    }

    screen.drawMenu(menuIndex, true);
}

void handleButtonEvent(const ButtonEvent& event) {
    if (uiMode == UiMode::Dashboard) {
        if (event.type == ButtonEventType::ShortPress) {
            uiMode = UiMode::Menu;
            menuIndex = 0;
            screen.drawMenu(menuIndex, false);
            return;
        }

        if (event.type == ButtonEventType::LongPress) {
            leds.toggleSimpleLed();
            leds.cycleRgbColor();
            dashboardDirty = true;
        }
        return;
    }

    // Menu mode: short click navigates, long click executes.
    if (event.type == ButtonEventType::ShortPress) {
        if (event.buttonIndex == 2) {
            menuIndex = (menuIndex + 2) % 3; // previous item
        } else {
            menuIndex = (menuIndex + 1) % 3; // next item
        }
        screen.drawMenu(menuIndex, false);
        return;
    }

    runMenuAction();
}

// ── setup() runs once on power-on ─────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    display.begin();
    buttons.begin();
    leds.begin();
    servo.begin();
    sensor.begin();

    screen.begin(makeDashboardData());
}

// ── loop() runs forever ───────────────────────────────────────────────────
void loop() {
    ButtonEvent event;
    if (buttons.pollEvent(millis(), event)) {
        handleButtonEvent(event);
        dashboardDirty = true;
    }

    servo.update();

    bool b1 = buttons.isPressed(0);
    bool b2 = buttons.isPressed(1);
    bool b3 = buttons.isPressed(2);
    if (b1 != prevB1 || b2 != prevB2 || b3 != prevB3) {
        prevB1 = b1;
        prevB2 = b2;
        prevB3 = b3;
        dashboardDirty = true;
    }

    if (uiMode == UiMode::Dashboard && dashboardDirty) {
        drawDashboard();
    }

    if (sensor.updateIfDue(millis())) {
        if (uiMode == UiMode::Dashboard) {
            dashboardDirty = true;
        }
    }

    delay(20);
}
