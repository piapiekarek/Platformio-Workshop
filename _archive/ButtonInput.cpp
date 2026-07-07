#include "ButtonInput.h"

#include "pin.h"

namespace {
const uint8_t BUTTON_PINS[ButtonInput::BUTTON_COUNT] = {
    PIN_BUTTON_1,
    PIN_BUTTON_2,
    PIN_BUTTON_3,
};
}

void ButtonInput::begin() {
    for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
        bool raw = digitalRead(BUTTON_PINS[i]);
        _buttons[i].lastRaw = raw;
        _buttons[i].stableRaw = raw;
        _buttons[i].lastDebounceMs = millis();
        _buttons[i].pressStartMs = 0;
    }
}

bool ButtonInput::pollEvent(uint32_t nowMs, ButtonEvent& event) {
    event.buttonIndex = 0;
    event.type = ButtonEventType::None;

    for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
        if (updateOne(i, BUTTON_PINS[i], nowMs, event)) {
            return true;
        }
    }

    return false;
}

bool ButtonInput::isPressed(uint8_t index) const {
    if (index >= BUTTON_COUNT) {
        return false;
    }
    return _buttons[index].stableRaw == LOW;
}

bool ButtonInput::updateOne(uint8_t index, uint8_t pin, uint32_t nowMs, ButtonEvent& event) {
    ButtonState& state = _buttons[index];
    bool raw = digitalRead(pin);

    if (raw != state.lastRaw) {
        state.lastRaw = raw;
        state.lastDebounceMs = nowMs;
    }

    if (nowMs - state.lastDebounceMs < DEBOUNCE_MS) {
        return false;
    }

    if (state.stableRaw == raw) {
        return false;
    }

    state.stableRaw = raw;
    if (raw == LOW) {
        state.pressStartMs = nowMs;
        return false;
    }

    uint32_t pressMs = nowMs - state.pressStartMs;
    event.buttonIndex = index;
    event.type = (pressMs >= LONG_PRESS_MS) ? ButtonEventType::LongPress : ButtonEventType::ShortPress;
    return true;
}
