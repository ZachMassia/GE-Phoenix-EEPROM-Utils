#pragma once

#define WRT 3
#define REC 4
#define AD7 6
#define AD4 7
#define AD3 8
#define AD2 9
#define AD1 10
#define AD0 11
#define STO 12
#define LED 13
#define IO1 A0
#define IO2 A1
#define IO3 A2
#define IO4 A3
#define AD5 A4
#define AD6 A5

#define BAUD_RATE   115200
#define DTIME       5 
#define RAM_SIZE    255
#define ADD_PIN_CNT 8
#define IO_PIN_CNT  4

enum MsgType {
  Read,
  Write,
  Print,
  Ack
};