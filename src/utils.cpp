#include "utils.hpp"


void writeAddr(byte addr, byte pins[ADD_PIN_CNT]) {
    for (int i = 0; i < ADD_PIN_CNT; i++) {
        digitalWrite(pins[i], bitRead(addr, i));
    }
    delayMicroseconds(DTIME);
}


void writeNibble(byte nibble, byte addr, byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT]) 
{
    writeAddr(addr, addr_pins);

    // Write nibble to io pins.
    for (int i = 0; i < IO_PIN_CNT; i++) {
        digitalWrite(io_pins[i], bitRead(nibble, i) > 0);
    }
    togglePin(WRT);
}

void togglePin(uint8_t pin) {
    delayMicroseconds(DTIME);
    digitalWrite(pin, LOW);
    delayMicroseconds(DTIME);
    digitalWrite(pin, HIGH);
    delayMicroseconds(DTIME);
}

byte readNibble(byte pins[IO_PIN_CNT]) {
    byte data = 0x0;

    for (int i = 0; i < IO_PIN_CNT; i++) {
        if (digitalRead(pins[i])) {
            bitSet(data, i);
        }
    }
    return data;
}

void packarray(unsigned char *buff, int len)
{ 
    unsigned char *packed;
    unsigned char byte;
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

    // Toggle Array Recall pin to transfer contents of EEPROM to RAM.
    togglePin(REC);

    for (byte addr = 0; addr < RAM_SIZE; addr++) {
        writeAddr(addr, addr_pins);
        buffer[addr] = readNibble(io_pins);
    }

    packarray(buffer, RAM_SIZE);
}

size_t writeFile(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT], const byte* buffer, size_t size) {
    size_t addr = 0;

    // Set the IO pins to output.
    for (int i = 0; i < IO_PIN_CNT; i++) {
        pinMode(io_pins[i], OUTPUT);
    }

    size_t i = 0;
    do {
        byte c = buffer[i++];
        byte l_nib = c & 0x0F;         // 0000[0000]
        byte h_nib = (c & 0xF0) >> 4;  // [0000]0000

        writeNibble(h_nib, addr++, io_pins, addr_pins);
        writeNibble(l_nib, addr++, io_pins, addr_pins);
    } while (addr < RAM_SIZE);

    togglePin(STO);
    delay(10); // Max store duration 10ms as per datasheet.

    return addr / 2;  // num bytes written. 
}