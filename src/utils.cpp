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

void readAll(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT]) {
    // Set the IO pins to input.
    Serial.println("Set IO pins INPUT");
    for (int i = 0; i < IO_PIN_CNT; i++) {
        pinMode(io_pins[i], INPUT);
    }



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

void writeFile(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT]) {
    // Set a longer serial timeout so there is enough time to paste the file contents.
    Serial.setTimeout(15*1000);

    // Set the IO pins to output.
    Serial.println("Set IO pins OUTPUT");
    for (int i = 0; i < IO_PIN_CNT; i++) {
        pinMode(io_pins[i], OUTPUT);
    }
    
    // Read in the file contents over serial.
    char file[MAX_ADDR];
    //Serial.println("Paste in file contents now");
    //Serial.readString().toCharArray(file, MAX_ADDR);

    String data = "0000000000000000000008000F00FF000B210B710A000A00EB71EB710A00EA0008F5080008000F00080008000F00FF00F8F0F80008000F00080008000F00FF00080008000F00FF0008000800FF00FF00088008000F00FF0008000800FF00FF00086008000F00FF0008000800FF00FF0008C008000F0FFF0F08000800FF00FF00";
    data.toCharArray(file, MAX_ADDR);

    Serial.println("Begin write to chip.");
    for (byte addr = 0; addr < MAX_ADDR; addr++) {
        writeAddr(addr, addr_pins);
        
        delayMicroseconds(3);

        digitalWrite(WRT, LOW);
        delayMicroseconds(3);
        digitalWrite(WRT, HIGH);
        delayMicroseconds(3);
    }

    // Toggle Store pin to copy from RAM to EEPROM section.
    Serial.println("Toggle store pin");
    digitalWrite(STO, LOW);
    delayMicroseconds(3);
    digitalWrite(STO, HIGH);
    delayMicroseconds(3);

    Serial.println("File successfully written to chip.");
}