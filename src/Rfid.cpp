#include "Rfid.h"
#include "pin.h"

#if RFID_ENABLED

#include <SPI.h>
#include <MFRC522.h>
#include "SpiBus.h"

namespace {
    MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

    bool available = false;
    bool initPending = true;
    String lastCardId = "";

    unsigned long lastInitTryTime = 0;
    unsigned long lastMissingLogTime = 0;
    unsigned long lastPollTime = 0;
    unsigned long lastCardTime = 0;

    const unsigned long RETRY_INTERVAL_MS = 1000;
    const unsigned long POLL_INTERVAL_MS = 100;

    // How long a scanned card ID stays visible before it's hidden again.
    const unsigned long CARD_ID_DISPLAY_MS = 15000;
}

static void tryInit() {
    selectRfidBus();
    rfid.PCD_Init(RFID_SS_PIN, RFID_RST_PIN);

    byte version = rfid.PCD_ReadRegister(MFRC522::VersionReg);
    if (version == 0x00 || version == 0xFF) {
        available = false; // reader not responding -> retry later
    } else {
        available = true;
        initPending = false;
        Serial.print("RFID reader ready (VersionReg=0x");
        Serial.print(version, HEX);
        Serial.println(")");
    }

    selectDisplayBus(); // always hand SPI back to the display
}

static bool checkForCard(String &uid) {
    if (!available) {
        return false;
    }

    selectRfidBus();

    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        selectDisplayBus();
        return false;
    }

    // Convert the UID bytes into a readable hex string, e.g. "04A3F1"
    uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
        if (rfid.uid.uidByte[i] < 0x10) {
            uid += "0";
        }
        uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    selectDisplayBus();
    return true;
}

void rfidSetup() {
    available = false;
    initPending = true;
    // The RC522 is deliberately not initialized here yet, that happens in
    // rfidUpdate() instead. This keeps startup fast even if no RFID reader
    // is connected (yet).
}

void rfidUpdate() {
    unsigned long now = millis();

    if (initPending && (now - lastInitTryTime >= RETRY_INTERVAL_MS)) {
        lastInitTryTime = now;
        tryInit();

        if (!available && (now - lastMissingLogTime >= 5000)) {
            lastMissingLogTime = now;
            Serial.println("RFID: reader not detected yet (check VCC/GND/SCK/MOSI/MISO/SS/RST)");
        }
    }

    if (now - lastPollTime >= POLL_INTERVAL_MS) {
        lastPollTime = now;
        String cardId;
        if (checkForCard(cardId)) {
            lastCardId = cardId;
            lastCardTime = now;
            Serial.println("RFID CARD: " + lastCardId);
        }
    }

    // Hide the card ID again after it has been shown long enough
    if (lastCardId.length() > 0 && (now - lastCardTime >= CARD_ID_DISPLAY_MS)) {
        lastCardId = "";
    }
}

bool rfidIsAvailable() {
    return available;
}

String rfidLastCardId() {
    return lastCardId;
}

#else // RFID_ENABLED == 0: everything becomes a no-op, main.cpp needs no special cases

void rfidSetup() {}
void rfidUpdate() {}
bool rfidIsAvailable() { return false; }
String rfidLastCardId() { return ""; }

#endif
