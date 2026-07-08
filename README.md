# RP2350 Control Panel

Simple PlatformIO project for Pico 2 (RP2350) with:

- Waveshare 1.47" display (ST7789, 320x172)
- DS18B20 temperature sensor
- SG90 servo (sweep demo)
- 1 normal LED
- 1 addressable RGB LED (WS2812 or SK6812)
- up to 3 buttons for input and demo control

This repository is structured for beginners: each hardware block is in its own file.

## Examples For Presentation

The `examples/` folder contains three small step-by-step demos for live presentations:

- `examples/01_display_farbe.cpp` - simplest example: only changes the display background color
- `examples/02_if_for_demo.cpp` - middle example: shows `if` and `for` with a counter and bars
- `examples/03_main_komplex_kopie.cpp` - most complex example: copy of the current `src/main.cpp`

Suggested order for explaining the topic:

1. start with the color-only example
2. show the `if` / `for` example
3. finish with the full application in `src/main.cpp`

## Hardware Summary

- Board: Raspberry Pi Pico 2 (`rpipico2`)
- Framework: Arduino (Earle Philhower core)
- Environment in PlatformIO: `rp2350`

## Build And Upload

From the project root:

```bash
~/.platformio/penv/bin/platformio run -e rp2350
~/.platformio/penv/bin/platformio run -e rp2350 -t upload --upload-port /dev/ttyACM0
```

If `platformio` is already in your PATH, the `~/.platformio/penv/bin/` prefix is optional.

## Pin Mapping

Defined in `src/pin.h`. Only GP0-9 and GP25-29 are exposed on the RP2350 board:

- `PIN_BUTTON_1 = 0` (GP0)
- `PIN_BUTTON_2 = 1` (GP1)
- `PIN_BUTTON_3 = 2` (GP2)
- `PIN_DS18B20 = 3` (GP3)
- `PIN_LED_SIMPLE = 4` (GP4)
- `PIN_RGB_LED = 5` (GP5)
- `PIN_SERVO = 6` (GP6)

### Wiring Notes

- Buttons use `INPUT_PULLUP`:
  - one side to GPIO
  - other side to GND
- DS18B20:
  - DATA to `PIN_DS18B20`
  - VCC to 3V3
  - GND to GND
  - add pull-up resistor between DATA and 3V3 if your module does not include one
- Normal LED:
  - GPIO -> resistor (e.g. 220 ohm) -> LED -> GND
- WS2812/SK6812:
  - DATA to `PIN_RGB_LED`
  - power and GND according to your module
  - common GND with Pico 2 is required

## Buttons And Input

Implemented in `src/ButtonInput.cpp`:

- Debounce: `25 ms`
- Current simple workshop behavior: button pressed = LED on, button released = LED off
- The presentation examples in `examples/` are intentionally simpler and do not depend on the full app flow

## Display Views And Menu

Implemented in `src/AppScreen.cpp` and controlled from `src/main.cpp`.

### Dashboard

Shows:

- sensor status (`CONNECTED` / `UNCONNECTED`)
- button 1 state (`DOWN` / `UP`)
- normal LED state (`ON` / `OFF`)
- temperature value in large text

The layout uses clean boxes with good spacing to avoid text overlapping.

### Menu

Open menu from dashboard with **short click on Button 1**.

- **short click**: move selection
- **long click**: execute selected action

Menu actions:

- Toggle LED
- Back to dashboard

## Project Structure

### Display abstraction

- `src/Display.h`
- `src/Display.cpp`

### Input / output / sensors

- `src/ButtonInput.h`
- `src/ButtonInput.cpp`
- `src/LedControl.h`
- `src/LedControl.cpp`
- `src/TemperatureSensor.h`
- `src/TemperatureSensor.cpp`
- `src/ServoSweep.h`
- `src/ServoSweep.cpp`

### UI and app flow

- `src/AppScreen.h`
- `src/AppScreen.cpp`
- `src/main.cpp`

## RGB Type Configuration (WS2812 vs SK6812)

In `src/LedControl.cpp`:

```cpp
#ifndef RGB_LED_SK6812
#define RGB_LED_SK6812 0
#endif
```

- `0` -> WS2812 (GRB)
- `1` -> SK6812 (GRBW)

## Beginner Editing Guide

If you are new to Arduino, start here:

1. Change pin numbers in `src/pin.h`
2. Change RGB brightness in `src/LedControl.cpp` (`RGB_BRIGHTNESS`)
3. Change menu entries and layout in `src/AppScreen.cpp`
4. Keep `src/main.cpp` mostly as orchestration only

## Troubleshooting

### Display stays black

- check all display wires
- verify display power and backlight pins
- confirm you are building/uploading `-e rp2350`

### Sensor always unconnected

- verify DS18B20 wiring and pull-up resistor
- check GND continuity

### Button clicks do nothing

- confirm button wiring to GND (with `INPUT_PULLUP`)
- test with only `PIN_BUTTON_1` connected first
- if you use the simplified `src/main.cpp`, remember the LED follows the button state directly, so the button must stay pressed for the LED to remain on

### Upload fails

- check correct upload port (e.g. `/dev/ttyACM0`)
- reconnect USB cable
- retry upload command
