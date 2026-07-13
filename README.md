# RP2350 Control Panel

PlatformIO-Projekt für den Waveshare **RP2350-LCD-1.47-A** (Raspberry Pi Pico 2 mit fest
verbautem 1.47"-Display) mit:

- Waveshare 1.47" Display (ST7789V2, 320×172, fest auf dem Board verdrahtet)
- Taster
- LED
- DS18B20-Temperatursensor
- RFID-Leser RC522 (SPI, extern angeschlossen, optional)
- WS2812-LED-Streifen (FastLED, extern angeschlossen, optional)

Dieses Repository ist für Workshops/Unterricht gedacht: `src/main.cpp` ist bewusst als
ein einziger, linearer Ablauf geschrieben (kein Klassen-Wirrwarr), Kommentare auf Deutsch.

## Examples für Präsentationen

Der Ordner `examples/` enthält kleine Schritt-für-Schritt-Demos für Live-Vorführungen.
Jedes Beispiel zeigt **ein** Bauteil für sich, ohne den Rest der Anwendung:

- `examples/01_display_farbe.cpp` – einfachstes Beispiel: setzt nur die Hintergrundfarbe
- `examples/02_if_for_demo.cpp` – mittleres Beispiel: `if` und `for` mit Zähler und Balken
- `examples/03_main_komplex_kopie.cpp` – 1:1-Kopie der vollen Anwendung aus `src/main.cpp`
  (Taster, LED, Temperatursensor, RFID, WS2812) als Referenz zum Nachschlagen; wird nicht
  automatisch synchron gehalten, falls `src/main.cpp` sich ändert
- `examples/04_bitmap_image.cpp` – eigenes Bild (Bitmap) auf dem Display anzeigen
- `examples/05_button_led.cpp` – nur Taster + LED, **ohne** Display
- `examples/06_temperature_display.cpp` – nur DS18B20-Temperatursensor, Wert auf dem Display
- `examples/07_ws2812_strip.cpp` – nur WS2812-LED-Kette (10 LEDs) mit FastLED, laufender Regenbogen
- `examples/08_rfid_reader.cpp` – nur RFID-Leser (RC522), Karten-ID auf dem seriellen Monitor

Empfohlene Reihenfolge für den Unterricht:

1. Farb-Beispiel (01)
2. `if`/`for`-Beispiel (02)
3. Einzelne Bauteile für sich: Taster+LED (05), Temperatursensor (06), WS2812 (07), RFID (08)
4. Bitmap-Bilder (04)
5. Alles zusammen: die volle Anwendung in `src/main.cpp`

Um eines der Beispiele zu bauen, trage es statt `main.cpp` im `build_src_filter` in
`platformio.ini` ein (oder tausche den Dateiinhalt aus).

## Hardware

- Board: Waveshare RP2350-LCD-1.47-A (Raspberry Pi Pico 2 / RP2350)
- Framework: Arduino (Earle Philhower Core)
- PlatformIO-Environment: `rp2350`

## Bauen und Flashen

Im Projektverzeichnis:

```bash
~/.platformio/penv/bin/platformio run -e rp2350
~/.platformio/penv/bin/platformio run -e rp2350 -t upload --upload-port /dev/ttyACM0
```

Wenn `platformio` bereits im PATH ist, kann der Präfix `~/.platformio/penv/bin/` entfallen.

## Pin-Belegung

Das Board hat nur **GPIO 0–9** als freien Header für eigene Bauteile. GPIO 10–22 sind
intern fest verdrahtet (Display, SD-Karte, RGB-LED) und dürfen für eigene Bauteile nicht
verwendet werden.

### Fest verdrahtet (Board-intern, siehe `platformio.ini` build_flags)

| Funktion   | GPIO |
|------------|------|
| SD SCLK    | 10   |
| SD MOSI    | 11   |
| SD MISO    | 12   |
| SD D1      | 13   |
| SD D2      | 14   |
| SD CS      | 15   |
| LCD DC     | 16   |
| LCD CS     | 17   |
| LCD CLK    | 18   |
| LCD DIN    | 19   |
| LCD RST    | 20   |
| LCD BL     | 21   |
| RGB IO     | 22   |

### Frei verdrahtet (eigene Bauteile, siehe `src/pin.h`)

| Bauteil            | Konstante        | GPIO |
|--------------------|-------------------|------|
| Taster             | `BUTTON_PIN`      | 1    |
| LED                | `LED_PIN`         | 4    |
| DS18B20 (Data)     | `TEMP_SENSOR_PIN` | 3    |
| RC522 MISO         | `RFID_MISO_PIN`   | 0    |
| RC522 SS/SDA       | `RFID_SS_PIN`     | 5    |
| RC522 SCK          | `RFID_SCK_PIN`    | 6    |
| RC522 MOSI         | `RFID_MOSI_PIN`   | 7    |
| RC522 RST          | `RFID_RST_PIN`    | 8    |
| WS2812 DATA        | `RGB_LED_PIN`     | 2    |

**Wichtig:** SCK und MOSI des RC522 lassen sich nicht auf beliebige GPIOs legen. Der
RP2350 hat für den SPI0-Bus feste Pin-Gruppen (Hardware-Multiplexer, kein freies
Remapping):

- SCK nur auf GPIO 2, 6, 18 oder 22
- MOSI (TX) nur auf GPIO 3, 7, 19 oder 23
- MISO (RX) nur auf GPIO 0, 4, 16 oder 20
- CS nur auf GPIO 1, 5, 17 oder 21

Wird ein ungültiger Pin gesetzt, hängt sich der Chip beim Start auf (SDK-`panic()`).
Wer die RC522-Pins ändern will, muss aus diesen Listen wählen.

### Verkabelungshinweise

- Taster: `INPUT_PULLUP`, eine Seite an GPIO, andere Seite an GND
- DS18B20: DATA an `TEMP_SENSOR_PIN`, VCC an 3V3, GND an GND (Pull-up-Widerstand
  zwischen DATA und 3V3 ergänzen, falls das Modul keinen eingebaut hat)
- LED: GPIO → Vorwiderstand (z. B. 220 Ω) → LED → GND
- RC522: VCC an 3V3 (**nicht 5V!**), restliche Pins siehe Tabelle oben
- WS2812-Streifen: DATA an `RGB_LED_PIN`, VCC/GND passend zum Streifen (bei mehr als
  ein paar LEDs eigene Stromversorgung verwenden, nicht über den Pico), gemeinsame
  Masse (GND) mit dem Pico ist Pflicht

## SPI-Bus wird geteilt (Display + RFID)

Display und RC522 hängen an unterschiedlichen GPIOs, nutzen aber denselben SPI0-Baustein
im Chip. `src/SpiBus.cpp` schaltet deshalb vor jedem Zugriff per `SPI.setTX/setSCK/setRX/setCS`
auf die passenden Pins um (`selectDisplayBus()` / `selectRfidBus()`) und deaktiviert das
jeweils andere Gerät über seinen CS-Pin. Diese Logik nicht "vereinfachen" oder entfernen –
sonst blockieren sich Display und RFID-Leser gegenseitig.

`TFT_MISO=255` in `platformio.ini` ist kein echter Pin, sondern der `NOPIN`-Wert: Das
Display hat keine MISO-Leitung, und alle vier für SPI0 gültigen MISO-Pins (0/4/16/20)
sind bereits von LED, RC522, DC und RST belegt.

## Optionale Bauteile per Präprozessorflag abschalten

Nicht jede Gruppe hat RFID oder einen WS2812-Streifen verbaut. Beides lässt sich in
`src/pin.h` einzeln deaktivieren:

```cpp
#define RFID_ENABLED 1     // auf 0 setzen, falls kein RC522 angeschlossen ist
#define RGB_LED_ENABLED 1  // auf 0 setzen, falls kein WS2812-Streifen angeschlossen ist
```

Bei `0` werden die zugehörigen Funktionen (`rfidSetup()`, `rgbLedFill()`, …) zu
No-ops übersetzt – `src/main.cpp` muss dafür **nicht** angepasst werden, die Aufrufe
bleiben einfach stehen.

## Ablauf in `src/main.cpp`

- Taster: LED folgt direkt dem Tasterzustand (an solange gedrückt, aus beim Loslassen)
- Taster (langer Druck, `BUTTON_LONG_PRESS_MS`): freie Stelle für eigenen Code
  (`handleButton()`), z. B. für die WS2812-LEDs
- Temperatur wird alle 1,5 s neu gelesen (`TEMP_READ_INTERVAL_MS`)
- RFID-Leser wird beim Start im Hintergrund initialisiert (kein Blockieren, falls kein
  Leser angeschlossen ist) und danach alle 100 ms nach einer Karte abgefragt
- Alle Werte werden im Display-Dashboard angezeigt: Sensor-Status, Taster-Status,
  LED-Status, Temperatur, RFID-Status (grau=aus, rot=Leser fehlt, grün=bereit, blau=Karte
  erkannt, Karten-ID steht rechts neben dem Punkt)
- Eine erkannte Karten-ID wird nach `CARD_ID_DISPLAY_MS` (`src/Rfid.cpp`, Standard 15s)
  automatisch wieder ausgeblendet

## Projektstruktur

- `src/main.cpp` – Programmablauf (setup/loop, Taster/LED/Temperatur, Anzeige)
- `src/pin.h` – frei verdrahtete Pins und Feature-Flags (`RFID_ENABLED`, `RGB_LED_ENABLED`)
- `src/Display.h` / `src/Display.cpp` – einfache Display-Klasse (Text, Formen, Balken, Farben)
- `src/SpiBus.h` / `src/SpiBus.cpp` – SPI-Bus zwischen Display und RFID umschalten
- `src/Rfid.h` / `src/Rfid.cpp` – kompletter RFID-Code (RC522), per `RFID_ENABLED` abschaltbar
- `src/RgbLed.h` / `src/RgbLed.cpp` – WS2812-Ansteuerung (FastLED), per `RGB_LED_ENABLED`
  abschaltbar
- `platformio.ini` – Board-Konfiguration und fest verdrahtete Display-Pins

## Displayfarben und Kontrast

`src/Display.h` bietet zwei Farbsätze:

- helle Farben (`GREEN`, `RED`, `CYAN`, `MAGENTA`, `GRAY`, `ORANGE`, …) – gut lesbar auf
  **schwarzem** Untergrund (siehe `examples/`)
- dunkle Varianten (`DARKGREEN`, `DARKRED`, `DARKCYAN`, `DARKMAGENTA`, `DARKGRAY`,
  `DARKORANGE`) – gut lesbar auf **weißem** Untergrund, wie im Dashboard in `main.cpp`

Faustregel: Text auf weißem Hintergrund → immer eine `DARK*`-Farbe verwenden.

## Einsteiger-Leitfaden

Wer neu mit Arduino/C++ ist, fängt hier an:

1. Pin-Nummern in `src/pin.h` anpassen (nur GPIO 0–9, siehe Einschränkungen oben)
2. `RFID_ENABLED` / `RGB_LED_ENABLED` in `src/pin.h` je nach eigenem Aufbau auf 0 oder 1 setzen
3. Layout/Texte im Dashboard in `updateDisplay()` (`src/main.cpp`) anpassen
4. Neue Aktionen für den langen Tasterklick in `handleButton()` ergänzen
5. Kalibrierung des Temperatursensors über `TEMP_CALIBRATION_OFFSET` anpassen

## Fehlersuche

### Display bleibt schwarz (Backlight an)

- Prüfen, ob `-e rp2350` gebaut/geflasht wurde
- Prüfen, ob `TFT_MISO` in `platformio.ini` auf `255` (NOPIN) steht, nicht auf einen
  echten Pin – ein ungültiger SPI-Pin lässt den Chip beim Start abstürzen

### RFID-Leser wird nicht erkannt

- Seriellen Monitor öffnen (115200 Baud): Meldung "RFID: reader not detected yet" =
  Leser antwortet nicht
- Verkabelung gegen die Tabelle oben prüfen, besonders SCK/MOSI (siehe Hinweis zu
  festen Pin-Gruppen)
- VCC des RC522 an 3V3, nicht 5V

### Sensor immer "UNCONNECTED"

- DS18B20-Verkabelung und Pull-up-Widerstand prüfen
- GND-Verbindung prüfen

### Taster reagiert nicht

- Verkabelung gegen GND prüfen (`INPUT_PULLUP`)
- `BUTTON_PIN` in `src/pin.h` mit tatsächlicher Verdrahtung abgleichen

### WS2812-Streifen bleibt aus

- `RGB_LED_ENABLED` in `src/pin.h` muss auf `1` stehen
- `RGB_LED_PIN` mit der tatsächlichen DATA-Verkabelung abgleichen
- `RGB_LED_COUNT` an die tatsächliche Anzahl LEDs im Streifen anpassen
- nach `rgbLedSetColor()`/`rgbLedFill()` muss `rgbLedShow()` aufgerufen werden, sonst
  bleibt die Änderung unsichtbar
- gemeinsame Masse (GND) zwischen Streifen und Pico prüfen

### Upload schlägt fehl

- richtigen Upload-Port prüfen (z. B. `/dev/ttyACM0`)
- USB-Kabel neu verbinden
- Upload-Befehl erneut ausführen
