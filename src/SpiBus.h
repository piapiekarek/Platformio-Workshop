#pragma once

// The display and the RFID reader are wired to different pins, but share the
// same SPI hardware inside the chip. That's why the pins must be reassigned
// with SPI.setXXX() every time before talking to one device or the other.
// The other device is disabled via its CS pin (HIGH) at the same time.

// Switch SPI to the display. Call before every display access.
void selectDisplayBus();

// Switch SPI to the RFID reader. Call before every RFID access.
void selectRfidBus();
