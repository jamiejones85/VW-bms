#include <Arduino.h>
#include "BMSCan.h"
#include <ACAN.h>
#include <ACAN2515.h>

ACAN2515* can3;
     
CANMessage BMSCan::convert(const BMS_CAN_MESSAGE &msg) {
  CANMessage ret;

  ret.id = msg.id;
  ret.len = msg.len;
  ret.ext = msg.flags.extended;

  for(int i = 0; i < msg.len; i++) {
    ret.data[i] = msg.buf[i];
  }
  return ret;
}

BMS_CAN_MESSAGE BMSCan::convert(const CANMessage &msg) {
  BMS_CAN_MESSAGE ret;

  ret.id = msg.id;
  ret.len = msg.len;
  ret.flags.extended = msg.ext;
  for(int i = 0; i < msg.len; i++) {
    ret.buf[i] = msg.data[i];
  }
  return ret;
}

int BMSCan::read (BMS_CAN_MESSAGE &msg) {
  CANMessage readMesg;
  int response = can3->receive(readMesg);
  msg = convert(readMesg);
  return response;
}

uint32_t BMSCan::available (void) {
  return can3->available();
}
void BMSCan::begin(uint32_t baud) {
   can3 = new ACAN2515 (MCP2515_CS, SPI, MCP2515_INT) ;
   ACAN2515Settings settings(16 * 1000 * 1000, baud);
   can3->begin(settings, [] { can3->isr () ; });
}

int BMSCan::write(const BMS_CAN_MESSAGE &msg) {
    CANMessage toSend = convert(msg);
    return can3->tryToSend(toSend);
}
