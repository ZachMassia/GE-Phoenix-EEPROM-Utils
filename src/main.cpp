/* Zachary Massia 2018 - 2019
 * Based off of https://github.com/KC7MMI/Arduino-GE-Phoenix-SX-EEPROM-Programmer/blob/e193ea63399f31dec88c2e295297498b78780c84/GE_PhoenixSX_Programmer_v1-01.ino
 * 
 * For programming the X2212 NVRAM chip in GE Phoenix SX radios
 */
#include <PacketSerial.h>

#include <Arduino.h>
#include "constants.h"
#include "utils.hpp"

PacketSerial myPacketSerial;

enum MsgType {
  Read,
  Write,
  Print
};

byte io_pins[IO_PIN_CNT] = {
  IO1, IO2, IO3, IO4
};

byte addr_pins[ADD_PIN_CNT] = {
  AD0, AD1, AD2, AD3, AD4, AD5, AD6, AD7
};

void onPacketReceived(const uint8_t* buffer, size_t size);


void setup() {
  myPacketSerial.begin(115200);
  myPacketSerial.setPacketHandler(&onPacketReceived);

  // Set the address pins to output. 
  for (int i = 0; i< ADD_PIN_CNT; i++) {
      pinMode(addr_pins[i], OUTPUT);
  }

  // Set the mode control pins to output.
  pinMode(WRT, OUTPUT);
  pinMode(STO, OUTPUT);
  pinMode(REC, OUTPUT);

  // Default all control pins to HIGH;
  digitalWrite(WRT, HIGH);
  digitalWrite(STO, HIGH);
  digitalWrite(REC, HIGH);


  /*Serial.println("Begining memory dump:");
  readAll(io_pins, addr_pins);

  writeFile(io_pins, addr_pins);

  Serial.println("Contents of chip after write:");
  readAll(io_pins, addr_pins);

  Serial.println("Done.");*/

}

void loop() { 
  myPacketSerial.update();
}

void onPacketReceived(const uint8_t* buffer, size_t size)
{
  byte msg_id = buffer[0];
  switch(msg_id) {
    case MsgType::Read: // ----------------------------------------------------
      byte data[MAX_ADDR + 1]; // Add extra byte for message ID. 
      readAll(io_pins, addr_pins, data + 1 ); // Send data buffer offset by 1 to prevent writing to ID byte.
      data[0] = MsgType::Print;
      myPacketSerial.send(data, MAX_ADDR + 1);
      break;
    case MsgType::Write: // ---------------------------------------------------
      writeFile(io_pins, addr_pins, buffer, size);
      break;
  }
}