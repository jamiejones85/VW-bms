#include <Arduino.h>
#include "BMSCan.h"
#include <FlexCAN.h> //https://github.com/collin80/FlexCAN_Library

CAN_message_t BMSCan::convert(const BMS_CAN_MESSAGE &msg) {
  CAN_message_t ret;

  ret.id = msg.id;
  ret.len = msg.len;

  for(int i = 0; i < msg.len; i++) {
    ret.buf[i] = msg.buf[i];
  }
  return ret;
}

BMS_CAN_MESSAGE BMSCan::convert(const CAN_message_t &msg) {
  BMS_CAN_MESSAGE ret;

  ret.id = msg.id;
  ret.len = msg.len;

  for(int i = 0; i < msg.len; i++) {
    ret.buf[i] = msg.buf[i];
  }
  return ret;
}

int BMSCan::read (BMS_CAN_MESSAGE &msg) {
  CAN_message_t readMesg;
  int response = Can0.read(readMesg);
  msg = convert(readMesg);
  return response;
}

uint32_t BMSCan::available (void) {
  return Can0.available();
}
void BMSCan::begin(uint32_t baud) {
   Can0.begin(baud);
   
   CAN_filter_t filter;

     //set filters for standard
  for (int i = 0; i < 8; i++)
  {
    //Can0.getFilter(filter, i);
    filter.flags.extended = 0;
    Can0.setFilter(filter, i);
  }
  //set filters for extended
  for (int i = 9; i < 13; i++)
  {
    //Can0.getFilter(filter, i);
    filter.flags.extended = 1;
    Can0.setFilter(filter, i);
  }
}

int BMSCan::write(const BMS_CAN_MESSAGE &msg) {
    CAN_message_t ret = convert(msg);
    return Can0.write(ret);
}
