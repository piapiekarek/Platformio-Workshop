// =============================================================================
//  Example: button + LED only (no display)
// =============================================================================
//
//  The simplest possible example: press the button, the LED turns on.
//  Release it, the LED turns off. No display needed for this one.
// =============================================================================

#include <Arduino.h>
#include "pin.h"

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP); // button wired to GND, resting state = HIGH
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    bool pressed = (digitalRead(BUTTON_PIN) == LOW);
    digitalWrite(LED_PIN, pressed ? HIGH : LOW);
}
