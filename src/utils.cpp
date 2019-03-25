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

void packarray(unsigned char *buff, int len)
{ 
    unsigned char *packed;
    unsigned char byte;
//    assert(len >= 2);  /* len must be at least 2 bytes */
//    assert((len & 1) != 1);   /* len must be an even number */
    for (packed = buff; len>0; len-=2) {
        byte= *buff++;
        *packed++ = (byte << 4) | *buff++;
    }
}

void readAll(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT], byte* buffer) {
    // Set the IO pins to input.
    for (int i = 0; i < IO_PIN_CNT; i++) {
        pinMode(io_pins[i], INPUT);
    }

    int dtime = 15;

    // Toggle Array Recall pin to transfer contents of EEPROM to RAM.
    digitalWrite(STO, LOW);
    delayMicroseconds(dtime);
    digitalWrite(STO, HIGH);
    delayMicroseconds(dtime);

    byte data[RAM_SIZE];
    for (byte addr = 0; addr < RAM_SIZE; addr++) {
        writeAddr(addr, addr_pins);
        delayMicroseconds(dtime);
        data[addr] = readNibble(io_pins);
    }
    
    memcpy(buffer, data, RAM_SIZE);
    packarray(buffer, RAM_SIZE);
}

void writeFile(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT], const byte* buffer, size_t size) {
    // Set a longer serial timeout so there is enough time to paste the file contents.
    Serial.setTimeout(15*1000);

    // Set the IO pins to output.
    for (int i = 0; i < IO_PIN_CNT; i++) {
        pinMode(io_pins[i], OUTPUT);
    }

    byte addr = 0;
    int dtime = 15;

    for (byte i = 1; i < size; i++) {
        char c = buffer[i];
        
        byte l_nib, h_nib;

        l_nib = c & 0x0F;         // 0000[0000]
        h_nib = (c & 0xF0) >> 4;  // [0000]0000

        writeAddr(addr++, addr_pins);
        delayMicroseconds(dtime);
        writeNibble(h_nib, io_pins);
        delayMicroseconds(dtime);

        digitalWrite(WRT, LOW);
        delayMicroseconds(dtime);
        digitalWrite(WRT, HIGH);
        delayMicroseconds(dtime);
 
        writeAddr(addr++, addr_pins);
        delayMicroseconds(dtime);
        writeNibble(l_nib, io_pins);
        delayMicroseconds(dtime);

        digitalWrite(WRT, LOW);
        delayMicroseconds(dtime);
        digitalWrite(WRT, HIGH);
        delayMicroseconds(dtime);
    }

    // Toggle Store pin to copy from RAM to EEPROM section.
    digitalWrite(STO, LOW);
    delayMicroseconds(dtime);
    digitalWrite(STO, HIGH);
    delay(10); // Max store duration 10ms as per datasheet.
}