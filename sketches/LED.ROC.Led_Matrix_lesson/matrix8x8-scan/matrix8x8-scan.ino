/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED Matrix backpacks 
  ----> http://www.adafruit.com/products/872
  ----> http://www.adafruit.com/products/871
  ----> http://www.adafruit.com/products/870

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

static const uint8_t PROGMEM box[] = {
    B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100
};

void setup() {
  Serial.begin(9600);
  Serial.println("8x8 LED Matrix Test");
  
  matrix.begin(0x70);  // pass in the address

  matrix.clear();
  matrix.drawBitmap(0, 0, box, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(2000);
  matrix.clear();
}

void loop() {
  matrix.clear();
  for(byte x=0; x<8; x++){
    if(x>0)
      matrix.drawLine(x-1, 0, x-1, 7, 0);
    matrix.drawLine(x, 0, x, 7, 1);
    matrix.writeDisplay();
    delay(100);
  }
  matrix.drawLine(7, 0, 7, 7, 0);
  for(byte y=0; y<8; y++){
    if(y>0)
      matrix.drawLine(0, y-1, 7, y-1, 0);
    matrix.drawLine(0, y, 7, y, 1);
    matrix.writeDisplay();
    delay(100);
  }
  matrix.drawLine(0, 7, 7, 7, 0);
}
