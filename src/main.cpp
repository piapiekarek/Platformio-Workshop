#include <Arduino.h>
#include "Display.h"

Display display;

void setup() {
    display.begin();
    display.setRotation(3);

    // Simplest example: just set the background color.
    display.clear(BLUE);
    display.print("Hello, World!", 10, 26, WHITE, 2);

}

void loop() {
    delay(1000);
}
