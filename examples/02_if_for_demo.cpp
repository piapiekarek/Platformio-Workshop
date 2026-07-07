#include <Arduino.h>
#include "Display.h"

Display display;

int zaehler = 0;

void setup() {
    display.begin();
    display.setRotation(3);
}

void loop() {
    display.clear(BLACK);

    // FOR: zeichnet Balken von links nach rechts.
    for (int i = 0; i < 10; i++) {
        int x = 10 + i * 30;
        int hoehe = 20 + i * 10;
        display.rectFilled(x, 160 - hoehe, 20, hoehe, CYAN);
    }

    // IF: unterscheidet zwischen gerader und ungerader Zahl.
    if (zaehler % 2 == 0) {
        display.print("Gerade Zahl", 10, 10, GREEN, 2);
    } else {
        display.print("Ungerade Zahl", 10, 10, ORANGE, 2);
    }

    display.print("Zaehler:", 10, 40, WHITE, 2);
    display.number((float)zaehler, 110, 40, YELLOW, 2, 0);

    zaehler++;
    if (zaehler > 20) {
        zaehler = 0;
    }

    delay(800);
}
