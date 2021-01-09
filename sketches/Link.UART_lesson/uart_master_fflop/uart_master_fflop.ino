#define CMD_LED 0xA1

#include <SerialFlow.h>

SerialFlow rd(&Serial);

const int btn = A2;
byte flag = 0;

void setup(){
  rd.setPacketFormat(1, 2); // размер одного значения в байтах, размер пакета в значениях, 
  rd.begin(9600);
  
  pinMode(btn, INPUT);
}

void loop(){
  rd.setPacketValue(CMD_LED);
  rd.setPacketValue(flag);
  rd.sendPacket();
  flag =! flag;
  delay(100);
}


