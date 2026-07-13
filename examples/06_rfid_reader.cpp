// =============================================================================
//  Example: RFID reader only (RC522)
// =============================================================================
//
//  Shows the ID of any card held near the RC522 reader right on the display.
//  (Also prints it to the Serial Monitor at 115200 baud, if you have it open.)
//
//  Display and RFID reader share the same SPI hardware on this board, so the
//  pins have to be switched with selectDisplayBus()/selectRfidBus() before
//  each access - see src/SpiBus.cpp for details. Don't skip that part if you
//  copy this example, or the display and the reader will fight over the bus.
// =============================================================================

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "Display.h"
#include "pin.h"
#include "SpiBus.h"

Display display;
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

bool readerFound = false;

void showStatus(const String& line1, const String& line2, uint16_t color) {
    selectDisplayBus();
    display.rectFilled(0, 0, 320, 172, WHITE);
    display.print("RFID Reader", 10, 10, BLACK, 2);
    display.print(line1, 10, 60, color, 2);
    if (line2.length() > 0) {
        display.print(line2, 10, 90, color, 2);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    display.begin();
    display.setRotation(3);
    showStatus("Starting...", "", DARKGRAY);

    selectRfidBus();
    rfid.PCD_Init(RFID_SS_PIN, RFID_RST_PIN);

    byte version = rfid.PCD_ReadRegister(MFRC522::VersionReg);
    readerFound = (version != 0x00 && version != 0xFF);

    if (readerFound) {
        Serial.println("RFID reader ready, hold a card near it...");
        showStatus("Hold a card near", "the reader", DARKGREEN);
    } else {
        Serial.println("RFID reader not found - check wiring!");
        showStatus("Reader not found!", "Check wiring.", DARKRED);
    }
}

void loop() {
    if (!readerFound) {
        return;
    }

    selectRfidBus();
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    // Convert the UID bytes into a readable hex string, e.g. "04A3F1"
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) {
            uid += "0";
        }
        uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    Serial.println("Card UID: " + uid);
    showStatus("Card found:", uid, DARKMAGENTA);

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(1500); // keep the ID on screen for a moment before scanning again
}
