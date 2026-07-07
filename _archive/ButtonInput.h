#pragma once

#include <Arduino.h>

enum class ButtonEventType : uint8_t {
    None = 0,
    ShortPress,
    LongPress,
};

struct ButtonEvent {
    uint8_t buttonIndex = 0;
    ButtonEventType type = ButtonEventType::None;
};

class ButtonInput {
public:
    static const uint8_t BUTTON_COUNT = 3;

    void begin();
    bool pollEvent(uint32_t nowMs, ButtonEvent& event);
    bool isPressed(uint8_t index) const;

private:
    struct ButtonState {
        bool lastRaw = true;
        bool stableRaw = true;
        uint32_t lastDebounceMs = 0;
        uint32_t pressStartMs = 0;
    };

    static const uint32_t DEBOUNCE_MS = 25;
    static const uint32_t LONG_PRESS_MS = 700;

    bool updateOne(uint8_t index, uint8_t pin, uint32_t nowMs, ButtonEvent& event);

    ButtonState _buttons[BUTTON_COUNT];
};
