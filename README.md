# Waveshare 1.47" LCD mit ESP32

Dieses Projekt zeigt, wie du ein **Waveshare 1.47" LCD-Display** (ST7789V2, 172×320 px) an einen ESP32 anschließt und mit PlatformIO programmierst. Die fertige `Display`-Klasse in `src/Display.h` macht das Zeichnen einfach – du rufst kurze, englische Funktionen auf und musst dich nicht um die interne Bibliothek kümmern.

---

## Inhaltsverzeichnis

1. [Was du brauchst](#was-du-brauchst)
2. [Software installieren](#software-installieren)
3. [Projekt öffnen](#projekt-öffnen)
4. [Verkabelung](#verkabelung)
5. [Welches Board hochladen?](#welches-board-hochladen)
6. [Display-Klasse benutzen](#display-klasse-benutzen)
7. [GUI mit Lopaka gestalten](#gui-mit-lopaka-gestalten)
8. [Häufige Fehler und Lösungen](#häufige-fehler-und-lösungen)
9. [Technische Details](#technische-details)

---

## Was du brauchst

| Teil | Beschreibung |
|------|-------------|
| **Waveshare 1.47" LCD Module** | ST7789V2-Treiber, 172×320 px, SPI-Schnittstelle |
| **ESP32-Board** | ESP32 WROOM 32 **oder** ESP32-C3 Zero **oder** ESP32-C6 Super Mini |
| **Jumperkabel** | Weiblich-Männlich, ca. 8 Stück |
| **USB-Kabel** | Passend zu deinem Board (Micro-USB oder USB-C) |
| **Computer** | Windows, macOS oder Linux |

---

## Software installieren

### 1. Visual Studio Code installieren
Lade [VS Code](https://code.visualstudio.com/) herunter und installiere es.

### 2. PlatformIO-Erweiterung installieren
1. VS Code öffnen
2. Links auf das **Erweiterungs-Symbol** (vier Quadrate) klicken
3. Nach **PlatformIO IDE** suchen → **Install** klicken
4. VS Code neu starten, wenn gefragt

> PlatformIO lädt alle benötigten Compiler und Bibliotheken automatisch herunter. Das dauert beim ersten Start ein paar Minuten.

---

## Projekt öffnen

1. Dieses Repository herunterladen (grüner **Code**-Button → *Download ZIP*) oder mit Git klonen:
   ```
   git clone <repo-url>
   ```
2. In VS Code: **File → Open Folder** → den heruntergeladenen Ordner auswählen
3. PlatformIO erkennt das Projekt automatisch an der Datei `platformio.ini`

---

## Verkabelung

Das Display hat **8 Pins**. Die folgende Tabelle zeigt, wie du jeden Pin verbindest.

> **Wichtig:** Immer zuerst verkabeln, dann erst den ESP32 per USB anschließen!

### ESP32 WROOM 32

```
LCD-Pin │ ESP32 GPIO │ Funktion
────────┼────────────┼──────────────────────────
VCC     │ 3V3        │ Stromversorgung (3,3 V)
GND     │ GND        │ Masse
DIN     │ GPIO 23    │ SPI-Daten (MOSI)
CLK     │ GPIO 18    │ SPI-Takt (Clock)
CS      │ GPIO 17    │ Chip Select
DC      │ GPIO 27    │ Data / Command
RST     │ GPIO  4    │ Reset
BL      │ GPIO 21    │ Hintergrundbeleuchtung
```

### ESP32-C3 Zero und ESP32-C6 Super Mini

```
LCD-Pin │ ESP32 GPIO │ Funktion
────────┼────────────┼──────────────────────────
VCC     │ 3V3        │ Stromversorgung (3,3 V)
GND     │ GND        │ Masse
DIN     │ GPIO  7    │ SPI-Daten (MOSI)
CLK     │ GPIO  6    │ SPI-Takt (Clock)
CS      │ GPIO 10    │ Chip Select
DC      │ GPIO  3    │ Data / Command
RST     │ GPIO  2    │ Reset
BL      │ GPIO  4    │ Hintergrundbeleuchtung
```

> **Hinweis für Anfänger:** Der Beschriftung auf dem Display-Modul folgen. `DIN` = Dateneingang, `CLK` = Takt, `CS` = Chip-Auswahl, `DC` = Daten/Befehl, `RST` = Reset, `BL` = Beleuchtung.

---

## Welches Board hochladen?

In VS Code siehst du unten in der blauen Statusleiste das aktuell gewählte Environment. Klicke darauf oder öffne die PlatformIO-Seitenleiste, um das passende Board auszuwählen:

| Board | Environment-Name |
|-------|-----------------|
| ESP32 WROOM 32 | `esp32wroom32` |
| ESP32-C3 Zero | `esp32c3zero` |
| ESP32-C6 Super Mini | `esp32c6supermini` |

**Upload-Schritte:**
1. Board über USB anschließen
2. Unten in VS Code auf den **Pfeil-Button** (→ Upload) klicken  
   *oder* über das PlatformIO-Menü: **Project Tasks → [dein Board] → Upload**
3. Beim ESP32 WROOM 32: Falls das Hochladen hängt, den **BOOT-Taster** auf dem Board gedrückt halten, bis „Connecting…" erscheint, dann loslassen

---

## Display-Klasse benutzen

Die gesamte Display-Logik steckt in `src/Display.h` und `src/Display.cpp`. Du arbeitest nur mit dem `Display`-Objekt in `src/main.cpp`.

### Aufbau von `src/main.cpp`

```cpp
#include <Arduino.h>
#include "Display.h"

Display display;   // Display-Objekt – einmal anlegen, überall benutzen

void setup() {
    Serial.begin(115200);
    display.begin();   // Display einschalten und initialisieren

    // Hier zeichnest du: Text, Formen, Zahlen ...
    display.clear(BLACK);
    display.print("Hello!", 10, 10, CYAN, 2);
}

void loop() {
    // Hier läuft alles, was sich wiederholt (z. B. Sensor auslesen)
    delay(500);
}
```

### Farb-Konstanten

```cpp
BLACK   WHITE   RED   GREEN   BLUE
YELLOW  CYAN    MAGENTA       GRAY    ORANGE
```

### Methoden der Display-Klasse

#### Initialisierung

```cpp
display.begin();
```
Einmal in `setup()` aufrufen. Schaltet die Hintergrundbeleuchtung ein und richtet das Display ein.

#### Bildschirm löschen

```cpp
display.clear();          // alles schwarz
display.clear(BLUE);      // alles blau
```

#### Text ausgeben

```cpp
//              Text       x    y    Farbe   Größe
display.print("Hallo!",   10,  10,  WHITE,  2);
```

- `x`, `y` – obere linke Ecke des Textes in Pixeln
- Größe: `1` = 8 px · `2` = 16 px · `3` = 24 px · `4` = 32 px

#### Zahl ausgeben

```cpp
//               Wert    x    y   Farbe  Größe  Dezimalstellen
display.number(22.5,    10,  50,  RED,   2,     1);
```

#### Formen zeichnen

```cpp
// Rechteck-Rahmen
display.rect(x, y, width, height, color);

// Gefülltes Rechteck
display.rectFilled(x, y, width, height, color);

// Kreis-Rahmen  (cx, cy = Mittelpunkt)
display.circle(cx, cy, radius, color);

// Gefüllter Kreis
display.circleFilled(cx, cy, radius, color);

// Linie von Punkt A nach Punkt B
display.line(x1, y1, x2, y2, color);
```

#### Fortschrittsbalken

Ideal für Sensorwerte (Temperatur, Helligkeit, Feuchtigkeit …):

```cpp
//            x    y   Breite  Höhe  Wert   Min    Max   Farbe
display.bar(  10, 145,  200,   18,  22.5,   0,    100,  GREEN);
```

### Vollständiges Beispiel

```cpp
#include <Arduino.h>
#include "Display.h"

Display display;

void setup() {
    Serial.begin(115200);
    display.begin();
    display.clear(BLACK);

    display.print("Temp:", 10, 10, WHITE, 2);

    float temperature = 22.5;
    display.number(temperature, 90, 10, RED, 2, 1);
    display.print("C", 145, 10, RED, 2);

    display.bar(10, 40, 200, 20, temperature, 0, 40, GREEN);

    display.rect(10, 80, 100, 50, WHITE);
    display.rectFilled(120, 80, 100, 50, BLUE);
    display.circle(270, 105, 30, YELLOW);
    display.line(0, 140, 319, 140, GRAY);
}

void loop() {
    // Aufgabe: Sensor auslesen und Wert anzeigen
    // int brightness = analogRead(34);
    // display.number(brightness, 10, 10, YELLOW, 2, 0);
    // display.bar(10, 40, 200, 20, brightness, 0, 4095, CYAN);
    delay(500);
}
```

---

## GUI mit Lopaka gestalten

[Lopaka](https://lopaka.app) ist ein kostenloses Browser-Tool, mit dem du grafische Oberflächen per Drag & Drop gestaltest und den fertigen C++-Code direkt exportierst.

### Schritt 1 – Lopaka öffnen

Gehe auf **https://lopaka.app** in deinem Browser. Keine Installation nötig.

### Schritt 2 – Richtiges Display einstellen

1. Klicke oben auf **Settings** (Zahnrad-Symbol)
2. Wähle als **Library**: `TFT_eSPI`
3. Stelle **Width** auf `320` und **Height** auf `172`  
   *(Das Display ist im Querformat 320×172 px)*
4. Bestätige mit **Apply** oder **OK**

### Schritt 3 – GUI gestalten

Auf der linken Seite siehst du Werkzeuge:

| Symbol | Funktion |
|--------|----------|
| **T** | Text hinzufügen |
| **□** | Rechteck zeichnen |
| **○** | Kreis zeichnen |
| **∕** | Linie zeichnen |
| **Bild** | Bitmap/Icon einfügen |

- Elemente auf der Zeichenfläche platzieren und verschieben
- Farben, Schriftgröße und Position im rechten Bereich anpassen
- Mit **Strg+Z** (Undo) Schritte rückgängig machen

### Schritt 4 – Code exportieren

1. Klicke oben rechts auf **Export** oder **Generate Code**
2. Wähle als Format: **TFT_eSPI**
3. Den angezeigten Code kopieren (Button **Copy**)

Der exportierte Code sieht ungefähr so aus:

```cpp
#include "MeinFont.h"

static const unsigned char PROGMEM mein_bild_bits[] = { 0x00, 0x1F, ... };

void drawScreen_1(void) {
    tft.fillScreen(0xE8EC);
    tft.drawBitmap(10, 10, mein_bild_bits, 30, 20, 0xFFFF);
    tft.drawString("Hallo!", 50, 80);
}
```

---

## Häufige Fehler und Lösungen

### Display bleibt schwarz
- Verkabelung prüfen – besonders VCC (muss 3,3 V sein, **nicht** 5 V!)
- Hintergrundbeleuchtung (BL) korrekt verbunden?
- Richtiges Board-Environment gewählt?

### `fatal error: Adafruit_GFX.h: No such file`
Lopaka hat `#include <Adafruit_GFX.h>` in eine Font-Datei geschrieben. Diese Zeile entfernen – TFT_eSPI beinhaltet alles Nötige.

### `redefinition of 'const GFXfont ...'`
Der Font (z. B. `FreeMono12pt7b`) ist doppelt vorhanden – einmal von TFT_eSPI, einmal als lokale Datei. Die lokale `.h`-Datei aus `src/` löschen und das `#include` in `main.cpp` entfernen.

### `sh: 1: -DTFT_MOSI=23: not found`
In `platformio.ini` stehen Kommentare (`;`) direkt hinter Build-Flags in der gleichen Zeile. Diese Inline-Kommentare entfernen – jedes Flag muss allein auf seiner Zeile stehen, ohne `;` danach.

### Upload schlägt fehl / „Connecting…" erscheint nicht
- Beim **ESP32 WROOM 32**: BOOT-Taster während des Uploads gedrückt halten
- Falschen COM-Port? In VS Code unten die Schnittstelle prüfen
- USB-Kabel tauschen (manche Kabel sind nur Ladekabel ohne Datenleitungen)

### Bild ist gespiegelt oder falsch gedreht
`tft.setRotation(x)` in `setup()` anpassen – Werte 0 bis 3 ausprobieren.

---

## Technische Details

### Bibliotheken

| Bibliothek | Version | Zweck |
|-----------|---------|-------|
| [TFT_eSPI (Bodmer)](https://github.com/Bodmer/TFT_eSPI) | 2.5.43+ | Display-Ansteuerung |

TFT_eSPI wird über PlatformIO automatisch installiert. Keine manuelle Installation nötig.

### Display-Einstellungen (automatisch via Build-Flags)

Die gesamte Konfiguration steht in `platformio.ini` und wird beim Kompilieren automatisch gesetzt. Es gibt **keine** `User_Setup.h`-Datei, die du bearbeiten müsstest.

| Einstellung | Wert |
|-------------|------|
| Treiber | ST7789_DRIVER |
| Auflösung | 172 × 320 px |
| SPI-Takt (Schreiben) | 40 MHz |
| SPI-Takt (Lesen) | 6 MHz |
| Hintergrundlicht-Logik | HIGH = an |

### Warum keine `User_Setup.h`?

TFT_eSPI erwartet normalerweise eine Konfigurationsdatei direkt in der Bibliothek. Das ist unpraktisch, weil sie bei Updates überschrieben wird. Stattdessen wird hier `-DUSER_SETUP_LOADED` als Build-Flag gesetzt – dadurch liest TFT_eSPI alle Einstellungen aus den `platformio.ini`-Flags. Das Projekt bleibt dadurch sauber und selbstständig.

### ESP32-C6 Besonderheit

Der ESP32-C6 benötigt `arduino-esp32` v3.x, was durch `platform = espressif32@^6.10.0` in `platformio.ini` sichergestellt wird. Außerdem nutzt er USB-CDC statt UART für die serielle Verbindung (`ARDUINO_USB_MODE=1`).
