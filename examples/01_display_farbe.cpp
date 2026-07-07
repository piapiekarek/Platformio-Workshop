#include <Arduino.h>
#include "Display.h"

Display display;

void setup() {
    display.begin();
    display.setRotation(3);

    // Einfachstes Beispiel: nur die Hintergrundfarbe setzen.
    display.clear(BLUE);
    display.print("Hello, World!", 10, 10, WHITE, 2);

}

void loop() {
    delay(1000);
}
