#include "SpiBus.h"
#include <Arduino.h>
#include <SPI.h>
#include "Display.h"
#include "pin.h"

void selectDisplayBus() {
    pinMode(RFID_SS_PIN, OUTPUT);
    digitalWrite(RFID_SS_PIN, HIGH); // disable RFID
    SPI.setTX(TFT_MOSI);
    SPI.setSCK(TFT_SCLK);
    #if defined(TFT_MISO) && (TFT_MISO >= 0)
    SPI.setRX(TFT_MISO);
    #endif
    SPI.setCS(TFT_CS);
    SPI.begin();
}

void selectRfidBus() {
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH); // disable display
    pinMode(RFID_SS_PIN, OUTPUT);
    digitalWrite(RFID_SS_PIN, HIGH);
    SPI.setTX(RFID_MOSI_PIN);
    SPI.setSCK(RFID_SCK_PIN);
    SPI.setRX(RFID_MISO_PIN);
    SPI.setCS(RFID_SS_PIN);
    SPI.begin();
}
