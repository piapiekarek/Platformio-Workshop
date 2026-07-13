#pragma once

#include <Arduino.h>

// All RFID (RC522) code, for groups that don't have an RFID reader:
// pin.h -> set RFID_ENABLED to 0, and this module does nothing anymore.
// main.cpp does not need to be changed for that.

// Call once in setup().
void rfidSetup();

// Call every loop(): tries to initialize the reader, then regularly checks
// for a card once it's ready.
void rfidUpdate();

// true once the RC522 has responded at startup.
bool rfidIsAvailable();

// UID of the last detected card as a hex string (e.g. "04A3F1B2"), or "".
String rfidLastCardId();
