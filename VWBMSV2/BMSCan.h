#pragma once
#include <Filters.h>//https://github.com/JonHub/Filters
#include <ACAN.h>

typedef struct BMS_CAN_MESSAGE {
    uint32_t id;
    uint16_t timestamp;
    struct {
        uint8_t extended:1; // identifier is extended (29-bit)
        uint8_t remote:1;   // remote transmission request packet type
        uint8_t overrun:1;  // message overrun
        uint8_t reserved:5;
    } flags;
    uint8_t len;          // length of data
    uint8_t buf[8];
} BMS_CAN_MESSAGE;


class BMSCan
{
  public:
     int write(const BMS_CAN_MESSAGE &msg);
     void BMSCan::begin(uint32_t baud);
     uint32_t available (void);
     int read (BMS_CAN_MESSAGE &msg);
     
  private:
     CANMessage convert(const BMS_CAN_MESSAGE &msg);
     BMS_CAN_MESSAGE convert(const CANMessage &msg);

};
