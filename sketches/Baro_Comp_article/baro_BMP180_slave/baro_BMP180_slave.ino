#define CMD_REQ 0xA1
#define CMD_ANS 0xA2

static const byte PACKET_SIZE = 3;
static const byte VALUE_SIZE = 4;
static const boolean SEPARATE_VALUES = false;

#include <Wire.h>
#include <SFE_BMP180.h>
#include <SerialFlow.h>

SFE_BMP180 pressure;
SerialFlow rd(&Serial);

const int led = 13;

void blink(int n, int d){
  for(int i=0; i<n; i++){
    digitalWrite(led, HIGH);
    delay(d);
    digitalWrite(led, LOW);
    delay(d);
  }
}

void setup(){
  rd.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
  rd.begin(9600);
  pressure.begin();
}

void loop(){
  byte cmd;
  double p,t;
  
  while( rd.receivePacket() ){
    cmd = rd.getPacket(0);
    if( cmd == CMD_REQ ){
      if( getValues(p,t) ){
        rd.setPacketValue(CMD_ANS);
        rd.setPacketValue((uint32_t)t);
        rd.setPacketValue((uint32_t)(p*100));
        rd.sendPacket();
      }
    }
  }
}

byte getValues(double &p, double &t) {
  char status;

  status = pressure.startTemperature();
  if (status != 0){
      delay(status);
      status = pressure.getTemperature(t);
      if (status != 0){
          status = pressure.startPressure(3); // 3 - max
          if (status != 0){
              delay(status);
              status = pressure.getPressure(p,t);
              if (status != 0){
                  return 1;
              }
          }
      }
  }
}


