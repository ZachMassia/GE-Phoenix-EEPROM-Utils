/* Zachary Massia 2018 - 2019
 * Based off of https://github.com/KC7MMI/Arduino-GE-Phoenix-SX-EEPROM-Programmer/blob/e193ea63399f31dec88c2e295297498b78780c84/GE_PhoenixSX_Programmer_v1-01.ino
 * 
 * For programming the X2212 NVRAM chip in GE Phoenix SX radios
 */

#include <Arduino.h>
#include "constants.h"
#include "utils.hpp"


void setup() {
  Serial.begin(115200);

  byte io_pins[] = {
      IO1, IO2, IO3, IO4
  };
  byte addr_pins[] = {
      AD0, AD1, AD2, AD3, AD4, AD5, AD6, AD7
  };

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


  Serial.println("Begining memory dump:");
  readAll(io_pins, addr_pins);

  writeFile(io_pins, addr_pins);

  Serial.println("Done.");
}

void loop() { }