#define CMD_LED 0xA1

#include <SerialFlow.h>

SerialFlow rd(&Serial);

const int btn = A2;

void setup(){
  rd.setPacketFormat(1, 2, 0); // размер одного значения в байтах, размер пакета в значениях, 
  rd.begin(9600);
  
  pinMode(btn, INPUT);
}

void loop(){
  if( digitalRead(btn) ){
    rd.setPacketValue(CMD_LED);
    rd.setPacketValue(1);
    rd.sendPacket();
  } else {
    rd.setPacketValue(CMD_LED);
    rd.setPacketValue(0);
    rd.sendPacket();
  }
  delay(100);
}


