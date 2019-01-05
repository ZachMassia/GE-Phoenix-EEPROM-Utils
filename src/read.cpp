#include "read.hpp"

void writeAddr(byte addr, byte pins[ADD_PIN_CNT]) {
    byte mask  = 0x01;
    for (int i = 0; i < ADD_PIN_CNT; i++) {
        digitalWrite(pins[i], (mask & addr) > 0);
        mask <<= 1;
    }
}

byte readNibble(byte pins[IO_PIN_CNT]) {
    byte data = 0x0;
    byte mask = 0x1;

    for (int i = 0; i < IO_PIN_CNT; i++) {
        if (digitalRead(pins[i])) {
            data |= mask;
        }
        mask <<= 1;
    }
    return data;
}

void readAll(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT]) {
    // Set the IO pins to input.
    Serial.println("Set IO pins INPUT");
    for (int i = 0; i < IO_PIN_CNT; i++) {
        pinMode(io_pins[i], INPUT);
    }

    // Set the address pins to output. 
    Serial.println("Set ADDR pins OUTPUT");
    for (int i = 0; i< ADD_PIN_CNT; i++) {
        pinMode(addr_pins[i], OUTPUT);
    }

    // Set the mode control pins to output.
    Serial.println("Set [WRT,STO,REC] OUTPUT");
    pinMode(WRT, OUTPUT);
    pinMode(STO, OUTPUT);
    pinMode(REC, OUTPUT);

    // Default all control pins to HIGH;
    Serial.println("Set [WRT,STO,REC] HIGH");
    digitalWrite(WRT, HIGH);
    digitalWrite(STO, HIGH);
    digitalWrite(REC, HIGH);

    // Toggle Array Recall pin to transfer contents of EEPROM to RAM.
    Serial.println("Toggle array recall pin");
    digitalWrite(STO, LOW);
    delayMicroseconds(3);
    digitalWrite(STO, HIGH);

    byte data[MAX_ADDR];
    Serial.print("Begin reading ");
    Serial.print(MAX_ADDR);
    Serial.println(" addresses..");
    for (byte addr = 0; addr < MAX_ADDR; addr++) {
        writeAddr(addr, addr_pins);
        delayMicroseconds(3);
        data[addr] = readNibble(io_pins);
    }
    // Print out data
    Serial.println("Printing data:");
    for (byte i = 0; i < MAX_ADDR; i++) {
        Serial.print(data[i], HEX);
    }
    Serial.println("\n\n- - - - - - - - - - - - - - - - - \n");
}