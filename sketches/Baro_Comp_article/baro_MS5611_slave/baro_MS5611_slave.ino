#define CMD_REQ 0xA1
#define CMD_ANS 0xA2

static const byte PACKET_SIZE = 3;
static const byte VALUE_SIZE = 4;
static const boolean SEPARATE_VALUES = false;

#include <Wire.h>
#include <MS5611.h>
#include <SerialFlow.h>

MS5611 ms5611;
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
  blink(2,100);

  pinMode(13, OUTPUT);
  while(!ms5611.begin()){
    delay(500);
  }
  ms5611.setOversampling(MS5611_ULTRA_HIGH_RES); // MS5611_ULTRA_LOW_POWER
  blink(3,100);
}

void loop(){
  byte cmd;

  while( rd.receivePacket() ){
    cmd = rd.getPacket(0);
    if( cmd == CMD_REQ ){
      //uint32_t rawTemp = ms5611.readRawTemperature();
      //uint32_t rawPressure = ms5611.readRawPressure();
      double tmp = ms5611.readTemperature();
      long pres = ms5611.readPressure();

      rd.setPacketValue(CMD_ANS);
      rd.setPacketValue((uint32_t)tmp);
      rd.setPacketValue((uint32_t)pres);
      rd.sendPacket();
    }
  }
}


