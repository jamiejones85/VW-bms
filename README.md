# VW-bms
Can based decoding of VW bms data.

Based on the SimpBMS code by Tom_EVNut https://github.com/Tom-evnut/VW-bms but modified to support a custom board design with 4 can bus transcivers, with 2 the native on a Teensy 3.6 and 2 SPI can buses. Also support for a Mitshubish Outlander Changer and a Tesla P100D SPI current shunt (WIP)

It also features a LM393 comparitor for Pilot Proxity detection to enable the AC on input. 

Board Design can be found https://easyeda.com/jamie84.jj/simpbmsvw 

This is opensource software and hardware design, no warranty given or implied. No guarantees of fitness for use. Any use is entirely at your own risk.
