#include <Arduino.h>
#include "constants.h"

void writeAddr(byte addr, byte pins[ADD_PIN_CNT]);

byte readNibble(byte pins[IO_PIN_CNT]);
void writeNibble(byte nibble, byte pins[IO_PIN_CNT]);

void readAll(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT]);
void writeFile(byte io_pins[IO_PIN_CNT], byte addr_pins[ADD_PIN_CNT]);
