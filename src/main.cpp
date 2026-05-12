/**
 * Waveshare 1.47" LCD – Demo
 * ──────────────────────────
 * Display  : ST7789V2, 172 × 320 px
 * Bibliothek: TFT_eSPI (Bodmer)
 * GUI-Tool : Lopaka  → https://lopaka.app
 *
 * Pin-Belegung und Board-Auswahl → platformio.ini
 *
 * Lopaka-Workflow:
 *   1. GUI unter https://lopaka.app/ gestalten
 *   2. Als Ausgabeformat "TFT_eSPI" wählen
 *   3. Exportierten Code in dieses Projekt einbinden:
 *        - Dateien in src/ oder include/ kopieren
 *        - Unten mit   #include "lopaka_ui.h"   einbinden
 *        - lopaka_render() im loop() aufrufen
 */

#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// ─── Lopaka-Export hier einbinden (nach GUI-Export) ──────────────────────────
// #include "lopaka_ui.h"

// ─────────────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    // Hintergrundbeleuchtung einschalten
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);

    // Display initialisieren
    tft.init();
    tft.setRotation(0);        // 0 = Portrait  172 × 320 px
                               // 1 = Landscape 320 × 172 px
    tft.fillScreen(TFT_BLACK);

    // ── Demo-Anzeige ─────────────────────────────────────────────────────────

    // Rahmen
    tft.drawRect(2, 2, tft.width() - 4, tft.height() - 4, TFT_WHITE);

    // Titel (zentriert)
    tft.setTextDatum(TC_DATUM);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Waveshare", tft.width() / 2, 14);
    tft.drawString("1.47\" LCD",  tft.width() / 2, 36);

    // Status-Infos
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(8, 72);
    tft.printf("%dx%d ST7789V2\n", tft.width(), tft.height());
    tft.setCursor(8, 84);  tft.println("TFT_eSPI  [bereit]");
    tft.setCursor(8, 96);  tft.println("Lopaka    [bereit]");

    // Farbbalken
    const uint16_t farben[]  = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW};
    const int16_t  balkenB   = tft.width() / 4;
    for (uint8_t i = 0; i < 4; i++) {
        tft.fillRect(i * balkenB, 120, balkenB, 28, farben[i]);
    }

    // Farbverlauf-Balken (16-bit Spektrum)
    for (int16_t x = 0; x < tft.width(); x++) {
        uint16_t farbe = tft.color565(
            map(x, 0, tft.width(), 0, 255),
            0,
            map(x, 0, tft.width(), 255, 0)
        );
        tft.drawFastVLine(x, 158, 20, farbe);
    }

    Serial.printf("Display bereit: %d x %d px\n", tft.width(), tft.height());
}

// ─────────────────────────────────────────────────────────────────────────────
void loop() {
    // Lopaka-generierten GUI-Code hier aufrufen:
    // lopaka_render();

    delay(1000);
}
