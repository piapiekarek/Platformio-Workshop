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

    _display.rectFilled(8, Y_STATUS, 304, 12, BLACK);
    _display.print("Sensor:", 10, Y_STATUS, WHITE, 1);
    _display.print(data.sensorConnected ? "connected" : "unconnected", 62, Y_STATUS,
                   data.sensorConnected ? GREEN : RED, 1);

    _display.rectFilled(8, Y_BUTTONS, 304, 14, BLACK);
    _display.print("B1:", 10, Y_BUTTONS, CYAN, 1);
    _display.print(buttonWord(data.button1Pressed), 34, Y_BUTTONS, WHITE, 1);
    _display.print("B2:", 96, Y_BUTTONS, CYAN, 1);
    _display.print(buttonWord(data.button2Pressed), 120, Y_BUTTONS, WHITE, 1);
    _display.print("B3:", 182, Y_BUTTONS, CYAN, 1);
    _display.print(buttonWord(data.button3Pressed), 206, Y_BUTTONS, WHITE, 1);

    _display.rectFilled(8, Y_IO, 304, 14, BLACK);
    _display.print("LED:", 10, Y_IO, WHITE, 1);
    _display.print(data.ledOn ? "ON" : "OFF", 36, Y_IO, data.ledOn ? GREEN : GRAY, 1);
    _display.print("RGB:", 92, Y_IO, WHITE, 1);
    _display.print(data.rgbName, 122, Y_IO, ORANGE, 1);

    _display.rectFilled(8, Y_TEMP_VALUE, 120, 20, BLACK);
    _display.number(data.temperatureC, 10, Y_TEMP_VALUE, YELLOW, 2, 1);
    _display.print("C", 76, Y_TEMP_VALUE, YELLOW, 2);
    _display.bar(148, Y_BAR, 160, 14, data.temperatureC, -10.0f, 50.0f, GREEN);
}

void AppScreen::drawMenu(uint8_t selectedItem, bool longHint) {
    if (_view != AppView::Menu) {
        _view = AppView::Menu;
        _display.clear(BLACK);
        drawMenuFrame();
    }

    const uint8_t index = selectedItem % 3;
    const uint16_t c0 = (index == 0) ? CYAN : WHITE;
    const uint16_t c1 = (index == 1) ? CYAN : WHITE;
    const uint16_t c2 = (index == 2) ? CYAN : WHITE;

    _display.rectFilled(18, Y_MENU_BOX + 12, SCREEN_W - 36, 70, BLACK);
    _display.print(index == 0 ? "> Toggle LED" : "  Toggle LED", 24, Y_MENU_ROW_1, c0, 1);
    _display.print(index == 1 ? "> Next RGB color" : "  Next RGB color", 24, Y_MENU_ROW_2, c1, 1);
    _display.print(index == 2 ? "> Back to dashboard" : "  Back to dashboard", 24, Y_MENU_ROW_3, c2, 1);

    _display.rectFilled(8, Y_MENU_FOOTER, SCREEN_W - 16, 24, BLACK);
    _display.print("Short click: next item", 10, Y_MENU_FOOTER, WHITE, 1);
    _display.print(longHint ? "Long click: run selected" : "Long click: action", 10, Y_MENU_FOOTER + 10, ORANGE, 1);
}

void AppScreen::drawDashboardFrame() {
    _display.print("Control Panel", 10, Y_HEADER, CYAN, 2);
    _display.line(0, 22, SCREEN_W - 1, 22, GRAY);

    _display.rect(6, 26, 308, 58, GRAY);
    _display.rect(6, 96, 130, 70, GRAY);
    _display.rect(142, 96, 172, 70, GRAY);

    _display.print("Buttons", 10, 88, CYAN, 1);
    _display.print("Temperature", 146, 88, CYAN, 1);
    _display.print("Value", 10, Y_TEMP_LABEL, WHITE, 1);
}

void AppScreen::drawMenuFrame() {
    _display.print("Button Menu", 10, Y_MENU_TITLE, CYAN, 2);
    _display.line(0, 24, SCREEN_W - 1, 24, GRAY);
    _display.rect(18, Y_MENU_BOX, SCREEN_W - 36, 86, GRAY);
}
