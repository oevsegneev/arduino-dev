//#include <Arduino.h>
#include <Wire.h>
#include "RTClib.h"
#include <TM1637Display.h>

#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);
RTC_DS3231 rtc;

byte tick = 0;

void setup(){
  display.setBrightness(0x0f);
  rtc.begin();
}

void loop(){
  DateTime now = rtc.now();

  if( tick )
    display.showNumberDecEx(now.hour(), (0x80 >> 3), true, 2, 0);
  else
    display.showNumberDecEx(now.hour(), 0, true, 2, 0);
  display.showNumberDec(now.minute(), true, 2, 2);

  tick = !tick;
  delay(1000);
}
