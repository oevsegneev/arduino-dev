#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

void setup(){
  display.setBrightness(0x0f);
}

void loop(){
  display.showNumberDec(12, false, 2, 0);
  display.showNumberDec(34, false, 2, 2);
  while(100);
}
