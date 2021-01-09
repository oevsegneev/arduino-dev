#include <Adafruit_NeoPixel.h>

#define PIN            6
#define NUMPIXELS      4

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte state = 0;

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  for(int v=0; v<255; v+=15){
      pixels.clear();
      if( state ){
        pixels.setPixelColor(0, pixels.Color(v,0,255-v));
        pixels.setPixelColor(2, pixels.Color(v,0,255-v));
      } else {
        pixels.setPixelColor(1, pixels.Color(v,0,255-v));
        pixels.setPixelColor(3, pixels.Color(v,0,255-v));
      }
      state = !state;
      pixels.show();
      delay(200);
  }
  for(int v=0; v<255; v+=15){
    for(int i=0;i<NUMPIXELS;i++){
      pixels.clear();
      if( state ){
        pixels.setPixelColor(0, pixels.Color(255-v,v,0));
        pixels.setPixelColor(2, pixels.Color(255-v,v,0));
      } else {
        pixels.setPixelColor(1, pixels.Color(255-v,v,0));
        pixels.setPixelColor(3, pixels.Color(255-v,v,0));
      }
      state = !state;
      pixels.show();
      delay(200);
    }
  }
  for(int v=0; v<255; v+=15){
    for(int i=0;i<NUMPIXELS;i++){
      pixels.clear();
      if( state ){
        pixels.setPixelColor(0, pixels.Color(0,255-v,v));
        pixels.setPixelColor(2, pixels.Color(0,255-v,v));
      } else {
        pixels.setPixelColor(1, pixels.Color(0,255-v,v));
        pixels.setPixelColor(3, pixels.Color(0,255-v,v));
      }
      state = !state;
      pixels.show();
      delay(200);
    }
  }
}
