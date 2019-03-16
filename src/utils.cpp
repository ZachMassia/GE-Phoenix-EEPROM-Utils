#include "utils.hpp"


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

void writeNibble(byte nibble, byte pins[IO_PIN_CNT]) {
    byte mask = 0x1;

    for (int i = 0; i < IO_PIN_CNT; i++) {
        digitalWrite(pins[i], (mask & nibble) > 0);
        mask <<= 1;
    }
}

void readAll(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT], byte* buffer) {
    // Set the IO pins to input.
    for (int i = 0; i < IO_PIN_CNT; i++) {
        pinMode(io_pins[i], INPUT);
    }

    // Toggle Array Recall pin to transfer contents of EEPROM to RAM.
    digitalWrite(STO, LOW);
    delayMicroseconds(3);
    digitalWrite(STO, HIGH);

    byte data[MAX_ADDR];
    for (byte addr = 0; addr < MAX_ADDR; addr++) {
        writeAddr(addr, addr_pins);
        delayMicroseconds(3);
        data[addr] = readNibble(io_pins);
    }
    
    memcpy(buffer, data, MAX_ADDR);
}

void writeFile(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT], const byte* buffer, size_t size) {
    // Set a longer serial timeout so there is enough time to paste the file contents.
    Serial.setTimeout(15*1000);

    // Set the IO pins to output.
    for (int i = 0; i < IO_PIN_CNT; i++) {
        pinMode(io_pins[i], OUTPUT);
    }

    for (byte addr = 1; addr < size; addr++) {
        writeAddr(addr, addr_pins);
        char c = buffer[addr];
        
        byte nibble_x, nibble_y;

        nibble_x = c >> 4;    // 0b[1111]0000
        nibble_y = c & 0x0F;  // 0b1111[0000]

        writeNibble(nibble_x, io_pins);
        delayMicroseconds(3);

        digitalWrite(WRT, LOW);
        delayMicroseconds(3);
        digitalWrite(WRT, HIGH);
        delayMicroseconds(3);
 
        writeNibble(nibble_y, io_pins);
        delayMicroseconds(3);

        digitalWrite(WRT, LOW);
        delayMicroseconds(3);
        digitalWrite(WRT, HIGH);
        delayMicroseconds(3);
    }

    // Toggle Store pin to copy from RAM to EEPROM section.
    digitalWrite(STO, LOW);
    delayMicroseconds(3);
    digitalWrite(STO, HIGH);
    delayMicroseconds(3);
}