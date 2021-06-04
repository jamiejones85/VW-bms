#include <Arduino.h>
#include "BMSCan.h"
#include <ACAN.h>
#include <ACAN2515.h>

ACAN2515* can2;
ACAN2515* can3;
bool started[] = {false, false, false, false};
     
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

int BMSCan::read (BMS_CAN_MESSAGE &msg, int interfaceIndex) {
  CANMessage readMesg;
  int response;
  if (interfaceIndex == 0) {
    response = ACAN::can0.receive(readMesg);
  } else if (interfaceIndex == 1) {
    response = ACAN::can1.receive(readMesg);
  } else if (interfaceIndex == 2) {
    response = can2->receive(readMesg);
  } else if (interfaceIndex == 3) {
    response = can3->receive(readMesg);
  }
  msg = convert(readMesg);
  return response;
}

uint32_t BMSCan::available (int interfaceIndex) {

  if (interfaceIndex == 0 && started[interfaceIndex]) {
    return ACAN::can0.available();
  } else if (interfaceIndex == 1 && started[interfaceIndex]) {
    return ACAN::can1.available();
  } else if (interfaceIndex == 2 && started[interfaceIndex]) {
    return can2->available();
  } else if (interfaceIndex == 3 && started[interfaceIndex]) {
    return can3->available();
  }
  return 0;
}
void BMSCan::begin(uint32_t baud, int interfaceIndex) {
   
  if (interfaceIndex == 0 && !started[interfaceIndex]) {
    ACANSettings settings(baud);
    ACAN::can0.begin(settings);
    started[interfaceIndex] = true;
  } else if (interfaceIndex == 1 && !started[interfaceIndex]) {
    ACANSettings settings(baud);
    ACAN::can1.begin(settings);
    started[interfaceIndex] = true;
  } else if (interfaceIndex == 2 && !started[interfaceIndex]) {
   can2 = new ACAN2515 (MCP2515_CS, SPI, MCP2515_INT) ;
   ACAN2515Settings settings(16 * 1000 * 1000, baud);
   can2->begin(settings, [] { can2->isr () ; });
   started[interfaceIndex] = true;
  } else if (interfaceIndex == 3 && !started[interfaceIndex]) {
   can3 = new ACAN2515 (MCP2515_CS_2, SPI1, MCP2515_INT_2) ;
   ACAN2515Settings settings(16 * 1000 * 1000, baud);
   can3->begin(settings, [] { can3->isr () ; });
   started[interfaceIndex] = true;
  }

}

int BMSCan::write(const BMS_CAN_MESSAGE &msg, int interfaceIndex) {
  CANMessage toSend = convert(msg);

  if (interfaceIndex == 0) {
    ACAN::can0.tryToSend(toSend);
  } else if (interfaceIndex == 1) {
    ACAN::can1.tryToSend(toSend); 
  } else if (interfaceIndex == 2 && can2 != NULL) {
    can2->tryToSend(toSend);
  } else if (interfaceIndex == 3 && can3 != NULL) {
    can3->tryToSend(toSend);
  }
  return 0;
}
