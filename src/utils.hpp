#pragma once

#include <Arduino.h>
#include <PacketSerial.h>
#include "constants.h"

void writeAddr(byte addr, byte pins[ADD_PIN_CNT]);

byte readNibble(byte pins[IO_PIN_CNT]);

void writeNibble(byte nibble, byte addr, byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT]);

void readAll(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT], byte* buffer);

size_t writeFile(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT], 
               const byte* buffer, size_t size);

void togglePin(uint8_t pin);
