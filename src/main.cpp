#include <Arduino.h>
#include "Display.h"

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

Display display;   // create the display object once – use it everywhere

// ── setup() runs once on power-on ─────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    display.begin();          // power on and initialise the display

    // ── Background ───────────────────────────────────────────────────────
    display.clear(BLACK);

    // ── Print text ───────────────────────────────────────────────────────
    //   display.print("text", x, y, colour, size);
    //   size: 1 = small (8 px)  2 = medium (16 px)  3 = large (24 px)
    display.print("Hello Workshop!", 10, 10, CYAN, 2);

    // ── Draw shapes ──────────────────────────────────────────────────────
    display.rect(10, 40, 80, 40, WHITE);             // outline rectangle
    display.rectFilled(110, 40, 80, 40, BLUE);       // filled rectangle
    display.circle(260, 60, 30, YELLOW);             // circle outline
    display.circleFilled(260, 60, 20, ORANGE);       // filled circle
    display.line(0, 100, 319, 100, GRAY);            // horizontal line

    // ── Show a sensor value as a number ──────────────────────────────────
    //   Replace the fixed value with a real sensor reading later
    float temperature = 22.5;
    display.print("Temp:", 10, 110, WHITE, 2);
    display.number(temperature, 90, 110, RED, 2, 1);   // 1 decimal place
    display.print("C", 145, 110, RED, 2);

    // ── Progress bar for a sensor value ──────────────────────────────────
    //   bar(x, y, width, height, value, min, max, colour)
    display.bar(10, 145, 200, 18, temperature, 0, 40, GREEN);
}

// ── loop() runs forever ───────────────────────────────────────────────────
void loop() {
    // ── Your task: ───────────────────────────────────────────────────────
    //
    // 1. Read a sensor value, e.g.:
    //      int brightness = analogRead(34);   // photoresistor on GPIO 34
    //
    // 2. Show the value on the display:
    //      display.number(brightness, 10, 110, YELLOW, 2, 0);
    //
    // 3. Update the progress bar:
    //      display.bar(10, 145, 200, 18, brightness, 0, 4095, CYAN);
    //
    // Tip: Only redraw the area that changed – otherwise the screen flickers.

    delay(500);   // short pause between readings
}
