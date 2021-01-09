//#define DEBUG 1

#define CMD_REQ 0xA1
#define CMD_ANS 0xA2

#define ST_READY 0
#define ST_WAIT 1

static const byte PACKET_SIZE = 3;
static const byte VALUE_SIZE = 4;
static const boolean SEPARATE_VALUES = false;

#include <SerialFlow.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

SFE_BMP180 pressure;
SerialFlow rd1(&Serial3);
SerialFlow rd2(&Serial2);

unsigned long tm;
unsigned long req_tm = 0;
unsigned long req_to = 100;

byte state = ST_READY;
uint32_t vals[3][2];

const int chipSelect = 53;
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
  if (!SD.begin(chipSelect)) {
    #ifdef DEBUG
    Serial.println("Card failed, or not present");
    #endif
    blink(3,500);
    return;
  }
  
  rd1.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
  rd2.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  
  rd1.begin(9600);
  rd2.begin(9600);
  
  blink(3,100);
  pressure.begin();
}

void loop(){
  byte cmd;
  double p,t;

  while( rd1.receivePacket() ){
    cmd = rd1.getPacket(0);
    if( cmd == CMD_ANS ){
      vals[1][0] = rd1.getPacket(1); 
      vals[1][1] = rd1.getPacket(2); 
    }
  }
  
  while( rd2.receivePacket() ){
    cmd = rd2.getPacket(0);
    if( cmd == CMD_ANS ){
      vals[2][0] = rd2.getPacket(1); 
      vals[2][1] = rd2.getPacket(2); 
    }
  }
  
  if( state == ST_READY ){

    rd1.setPacketValue(CMD_REQ);
    rd1.setPacketValue(0);
    rd1.setPacketValue(0);
    rd1.sendPacket();

    rd2.setPacketValue(CMD_REQ);
    rd2.setPacketValue(0);
    rd2.setPacketValue(0);
    rd2.sendPacket();

    if( getValues(p,t) ){
      vals[0][0] = (uint32_t)t;
      vals[0][1] = (uint32_t)(p*100);
    }
    state = ST_WAIT;
    req_tm = millis();
  }

  tm = millis();
  if( state == ST_WAIT && req_tm + req_to < tm ){
    state = ST_READY;
    #ifdef DEBUG
    Serial.print(vals[0][0]);
    Serial.print(";");
    Serial.print(vals[1][0]);
    Serial.print(";");
    Serial.print(vals[2][0]);
    Serial.print(";");
    Serial.print(vals[0][1]);
    Serial.print(";");
    Serial.print(vals[1][1]);
    Serial.print(";");
    Serial.print(vals[2][1]);
    Serial.println();
    #endif
    String dataString = "";
    dataString = String(vals[0][0])+";"+String(vals[1][0])+";"+String(vals[2][0])+";"+
                 String(vals[0][1])+";"+String(vals[1][1])+";"+String(vals[2][1]);

    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      #ifdef DEBUG
      Serial.println(dataString);
      #endif 
    }
    else {
      #ifdef DEBUG
      Serial.println("error opening datalog.txt");
      #endif
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
          status = pressure.startPressure(3); // 3-max
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


