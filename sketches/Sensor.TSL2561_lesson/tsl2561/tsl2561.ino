#include <Wire.h>
#include "TSL2561.h"

TSL2561 tsl(TSL2561_ADDR_FLOAT); 

void setup(void) {
  Serial.begin(9600);
  
  tsl.begin();
    
  tsl.setGain(TSL2561_GAIN_16X);
  
  tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS); //13, 101, 402 
}

void loop(void) {
  // This can take 13-402 milliseconds! Uncomment whichever of the following you want to read
  uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);     
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);
  

  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  
  Serial.print("Full: ");
  Serial.print(full);
  Serial.print(", Visible: ");
  Serial.print(full - ir);
  Serial.print(", Lux: ");
  Serial.println(tsl.calculateLux(full, ir));
  
  delay(100); 
}
