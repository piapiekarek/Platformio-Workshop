#include "AppScreen.h"

namespace {
static const int SCREEN_W = 320;
static const int SCREEN_H = 172;
static const int CORNER_RADIUS = 4;

// Layout constants
static const int Y_HEADER = 6;
static const int Y_SENSOR_BOX = 30;
static const int Y_BUTTON_BOX = 60;
static const int Y_LED_BOX = 90;
static const int Y_TEMP_BOX = 120;

// Menu constants
static const int Y_MENU_TITLE = 8;
static const int Y_MENU_BOX = 34;
static const int Y_MENU_FOOTER = 140;

const char* buttonWord(bool pressed) {
    return pressed ? "DOWN" : "UP";
}
}

void AppScreen::begin(const DashboardData& data) {
    _view = AppView::Dashboard;
    _display.setRotation(3);
    _display.clear(BLACK);
    drawDashboardFrame();
    drawDashboard(data);
}

void AppScreen::drawDashboard(const DashboardData& data) {
    if (_view != AppView::Dashboard) {
        _view = AppView::Dashboard;
        _display.clear(BLACK);
        drawDashboardFrame();
    }

    // ──────────────────────────────────────────────────────────────
    // Sensor Status Box (top, full width)
    _display.rectFilled(8, Y_SENSOR_BOX, SCREEN_W - 16, 22, BLACK);
    _display.print("Sensor: ", 12, Y_SENSOR_BOX + 2, WHITE, 1);
    _display.print(data.sensorConnected ? "CONNECTED" : "UNCONNECTED",
                   72, Y_SENSOR_BOX + 2,
                   data.sensorConnected ? GREEN : RED, 1);

    // ──────────────────────────────────────────────────────────────
    // Button 1 Status Box (left side)
    _display.rectFilled(8, Y_BUTTON_BOX, 148, 22, BLACK);
    _display.print("Button 1: ", 12, Y_BUTTON_BOX + 2, CYAN, 1);
    _display.print(buttonWord(data.button1Pressed),
                   84, Y_BUTTON_BOX + 2,
                   data.button1Pressed ? YELLOW : WHITE, 1);

    // ──────────────────────────────────────────────────────────────
    // LED Status Box (right side)
    _display.rectFilled(164, Y_BUTTON_BOX, SCREEN_W - 172, 22, BLACK);
    _display.print("LED: ", 168, Y_BUTTON_BOX + 2, WHITE, 1);
    _display.print(data.ledOn ? "ON" : "OFF",
                   204, Y_BUTTON_BOX + 2,
                   data.ledOn ? GREEN : GRAY, 1);

    // ──────────────────────────────────────────────────────────────
    // Temperature Value Box (bottom, full width)
    _display.rectFilled(8, Y_TEMP_BOX, SCREEN_W - 16, 38, BLACK);
    _display.print("Temperature", 12, Y_TEMP_BOX + 2, CYAN, 1);
    _display.number(data.temperatureC, 16, Y_TEMP_BOX + 14, YELLOW, 3, 1);
    _display.print("C", 78, Y_TEMP_BOX + 14, YELLOW, 3);
}

void AppScreen::drawMenu(uint8_t selectedItem, bool longHint) {
    if (_view != AppView::Menu) {
        _view = AppView::Menu;
        _display.clear(BLACK);
        drawMenuFrame();
    }

    const uint8_t index = selectedItem % 2;
    const uint16_t c0 = (index == 0) ? CYAN : WHITE;
    const uint16_t c1 = (index == 1) ? CYAN : WHITE;

    _display.rectFilled(18, Y_MENU_BOX + 12, SCREEN_W - 36, 50, BLACK);
    _display.print(index == 0 ? "> Toggle LED" : "  Toggle LED", 24, Y_MENU_BOX + 16, c0, 1);
    _display.print(index == 1 ? "> Back" : "  Back", 24, Y_MENU_BOX + 36, c1, 1);

    _display.rectFilled(8, Y_MENU_FOOTER, SCREEN_W - 16, 24, BLACK);
    _display.print("Short: navigate", 10, Y_MENU_FOOTER, WHITE, 1);
    _display.print(longHint ? "Long: execute" : "Long: action", 10, Y_MENU_FOOTER + 10, ORANGE, 1);
}

void AppScreen::drawDashboardFrame() {
    _display.print("Control Panel", 10, Y_HEADER, WHITE, 2);
    _display.line(0, 24, SCREEN_W - 1, 24, GRAY);

    // Draw rounded corner boxes for each section
    _display.rect(6, Y_SENSOR_BOX - 4, SCREEN_W - 12, 32, GRAY);
    _display.rect(6, Y_BUTTON_BOX - 4, 150, 32, GRAY);
    _display.rect(162, Y_BUTTON_BOX - 4, SCREEN_W - 172, 32, GRAY);
    _display.rect(6, Y_TEMP_BOX - 6, SCREEN_W - 12, 48, GRAY);
}

void AppScreen::drawMenuFrame() {
    _display.print("Button Menu", 10, Y_MENU_TITLE, WHITE, 2);
    _display.line(0, 24, SCREEN_W - 1, 24, GRAY);
    _display.rect(18, Y_MENU_BOX, SCREEN_W - 36, 66, GRAY);
}
