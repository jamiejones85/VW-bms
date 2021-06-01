#include <Arduino.h>
#include "BMSCan.h"
#include <ACAN.h>

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
  int response = ACAN::can0.receive(readMesg);
  msg = convert(readMesg);
  return response;
}

uint32_t BMSCan::available (void) {
  return ACAN::can0.available();
}
void BMSCan::begin(uint32_t baud) {
   ACANSettings settings(baud) ; // 125 kbit/s
   settings.mConfiguration = ACANSettings::k10_6_Filters;
   ACAN::can0.begin(settings);
}

int BMSCan::write(const BMS_CAN_MESSAGE &msg) {
    CANMessage toSend = convert(msg);
    return ACAN::can0.tryToSend(toSend);
}
