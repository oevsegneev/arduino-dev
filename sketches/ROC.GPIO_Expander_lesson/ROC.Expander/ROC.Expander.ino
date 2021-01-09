#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;
  
void setup() {
  mcp.begin(7);

  for(byte i=0; i<16; i++){
    mcp.pinMode(i, OUTPUT);
  }
}

void loop() {
  for(int i=0; i<8; i++){
    mcp.digitalWrite(i, HIGH);
    mcp.digitalWrite(15-i, HIGH);
    delay(30);
    if(i>0){
      mcp.digitalWrite(i-1, LOW);
      mcp.digitalWrite(15-i+1, LOW);
      delay(30);
    }
  }
  for(int i=7; i>=0; i--){
    mcp.digitalWrite(i, HIGH);
    mcp.digitalWrite(15-i, HIGH);
    delay(30);
    if(i<7){
      mcp.digitalWrite(i+1, LOW);
      mcp.digitalWrite(15-i-1, LOW);
      delay(30);
    }
  }
}
