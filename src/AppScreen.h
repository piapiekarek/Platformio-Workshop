#pragma once

#include "Display.h"

enum class AppView : uint8_t {
    Dashboard = 0,
    Menu,
};

struct DashboardData {
    bool sensorConnected = true;
    bool ledOn = false;
    const char* rgbName = "OFF";
    bool button1Pressed = false;
    bool button2Pressed = false;
    bool button3Pressed = false;
    float temperatureC = 0.0f;
};

class AppScreen {
public:
    explicit AppScreen(Display& display) : _display(display) {}

    void begin(const DashboardData& data);
    void drawDashboard(const DashboardData& data);
    void drawMenu(uint8_t selectedItem, bool longHint);

private:
    void drawDashboardFrame();
    void drawMenuFrame();

    AppView _view = AppView::Dashboard;
    Display& _display;
};
