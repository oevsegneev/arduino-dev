SerialFlow library for Arduino

Uses to transmit packed data.

You need to uncomment one of the #include directives in SerialFlow.h file in order to use nrf24 radio or FastSerial library.

For example:
// uncomment desired library
//#include <FastSerial.h>
//#include <RF24.h>

#include <HardwareSerial.h>