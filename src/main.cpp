// =============================================================================
//  Example: RFID reader only (RC522)
// =============================================================================
//
//  Prints the ID of any card held near the RC522 reader to the Serial Monitor
//  (115200 baud). No display needed for this one.
// =============================================================================

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "pin.h"

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

void setup() {
    Serial.begin(115200);
    delay(1000);

    SPI.setTX(RFID_MOSI_PIN);
    SPI.setSCK(RFID_SCK_PIN);
    SPI.setRX(RFID_MISO_PIN);
    SPI.setCS(RFID_SS_PIN);
    SPI.begin();

    rfid.PCD_Init();

    byte version = rfid.PCD_ReadRegister(MFRC522::VersionReg);
    if (version == 0x00 || version == 0xFF) {
        Serial.println("RFID reader not found - check wiring!");
    } else {
        Serial.println("RFID reader ready, hold a card near it...");
    }
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    Serial.print("Card UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) {
            Serial.print("0");
        }
        Serial.print(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(500); // avoid printing the same card many times per second
}
