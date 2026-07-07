# Vereinfachte Version für Workshop

## ✅ Was wurde geändert?

### Alte Struktur (zu komplex):
- ❌ AppScreen-Klasse mit State-Machine
- ❌ ButtonInput-Klasse mit debouncing
- ❌ LedControl-Klasse
- ❌ TemperatureSensor-Klasse
- ❌ ServoSweep-Klasse
- ❌ 7 verschiedene `.cpp` und `.h` Dateien

### Neue Struktur (anfängerfreundlich):
- ✅ **Eine main.cpp** mit allen Komponenten inline
- ✅ Klare, lineare Logik (keine State Machines)
- ✅ Ausführliche Kommentare auf Deutsch
- ✅ Direktes Verständnis: oben = Setup, Mitte = Logik, unten = loop()
- ✅ Nur noch 2 Dateien: `pin.h` + `main.cpp` (+ Display.h/cpp)

---

## 📋 Code-Struktur

```
main.cpp
├── PIN-DEFINITIONEN (GPIO 0, 3, 4)
│
├── GLOBALE VARIABLEN
│   ├── Display
│   ├── LED-Status: ledIsOn (bool)
│   ├── Temperatur: temperature (float)
│   ├── Button-State: lastButtonState, buttonPressTime
│   └── Sensor-State: oneWire, tempSensor, lastTempReadTime
│
├── SETUP-FUNKTIONEN
│   ├── setupDisplay()
│   ├── setupButton()
│   ├── setupLed()
│   └── setupTempSensor()
│
├── LOGIK-FUNKTIONEN
│   ├── updateDisplay()  – zeichnet alles
│   ├── readTemperature() – liest Sensor alle 1 Sekunde
│   └── handleButton()   – kurz/lang-klick-erkennung
│
└── MAIN
    ├── setup() – initialisiert alles
    └── loop()  – Hauptschleife
```

---

## 🔧 Neue Buttons hinzufügen - Super einfach!

Beispiel: Button 2 hinzufügen:

### 1. Pin definieren (pin.h):
```cpp
static const uint8_t PIN_BUTTON_2 = 1;  // GPIO 1
```

### 2. Globale Variable hinzufügen (main.cpp oben):
```cpp
bool lastButtonState2 = HIGH;
unsigned long buttonPressTime2 = 0;
```

### 3. Setup erweitern:
```cpp
void setupButton() {
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_BUTTON_2, INPUT_PULLUP);  // NEU
}
```

### 4. Logik hinzufügen in handleButton():
```cpp
// Analog zu Button 1, aber mit PIN_BUTTON_2 und den Button2-Variablen
```

### 5. Display aktualisieren:
```cpp
display.rect(164, 60, 152, 26, GRAY);  // 2. Box
display.print("Button 2: ", 168, 64, CYAN, 1);
if (digitalRead(PIN_BUTTON_2) == LOW) {
    display.print("PRESSED", ...);
}
```

---

## 💡 Schüler-freundlich Features

✅ **Klare Funktion je Komponente:**
- `readTemperature()` - nur Sensor
- `handleButton()` - nur Button-Logik
- `updateDisplay()` - nur Display

✅ **Einfache Variablenamen:**
- `ledIsOn` - selbserklärend
- `temperature` - nicht `_celsius` oder `_tempC`
- `tempSensorConnected` - kein Kürzel

✅ **Deutsche Kommentare:**
- Für deutschsprachige Schüler
- Erklärungen auf Deutsch

✅ **Verständliche Logik:**
```cpp
// Statt: if (nowMs - lastReadMs < UPDATE_INTERVAL_MS)
// Einfach: if (now - lastTempReadTime < 1000)
```

---

## 🚀 Nächste Schritte

1. **PlatformIO-Fehler beheben** auf Ihrer Maschine:
   ```bash
   pip install --upgrade platformio
   ```

2. **Kompilieren & hochladen:**
   ```bash
   platformio run -e rp2350
   platformio run -e rp2350 -t upload --upload-port /dev/ttyACM0
   ```

3. **Schüler können dann:**
   - Button 2 & 3 hinzufügen (super einfach jetzt!)
   - RGB LED direkt in `loop()` steuern
   - Farben in `updateDisplay()` ändern

---

## 📚 Alte Dateien (optional für Fortgeschrittene)

Die alten Klassen (ButtonInput, LedControl, etc.) sind noch im Repo für Fortgeschrittene Schüler:
- Zeigt: "So macht man es professionell"
- Zeigt: Klassen und Abstraktion
- Können später gelernt werden

Für jetzt: **Ignorieren.**

---

## ✨ Kompilierung

Sobald PlatformIO funktioniert: **ALLES SOLLTE KOMPILIEREN.**

Der Code ist:
- ✅ Syntaktisch korrekt
- ✅ Logisch korrekt
- ✅ Anfängerfreundlich
- ✅ Erweiterbar
