// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            12
#define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
IRTherm therm;

int delayval = 3; // delay for half a second

const int colormap[] ={1, 3, 74, 0, 3, 75, 0, 3, 76, 0, 3, 77, 0, 3, 82, 0, 7, 88, 0, 14, 94, 0, 22, 100, 0, 28, 106, 0, 35, 112, 0, 40, 119, 0, 45, 128, 0, 50, 134, 0, 52, 137, 0, 54, 142, 0, 56, 145, 0, 61, 154, 0, 65, 159, 0, 68, 164, 0, 71, 170, 0, 75, 179, 0, 78, 184, 0, 80, 188, 0, 84, 194, 0, 88, 200, 0, 92, 205, 0, 94, 208, 0, 96, 210, 0, 99, 214, 0, 103, 218, 0, 105, 220, 0, 109, 223, 0, 113, 223, 0, 117, 221, 1, 120, 219, 2, 124, 216, 3, 129, 212, 4, 132, 205, 4, 134, 197, 6, 138, 185, 8, 142, 172, 10, 144, 162, 12, 146, 153, 15, 149, 140, 22, 153, 120, 28, 156, 109, 40, 160, 94, 51, 164, 79, 67, 171, 60, 78, 175, 48, 89, 179, 39, 98, 183, 31, 109, 187, 23, 118, 189, 19, 128, 193, 14, 138, 196, 10, 146, 198, 6, 155, 201, 4, 164, 204, 2, 173, 206, 1, 178, 207, 1, 190, 210, 0, 196, 212, 0, 202, 213, 1, 212, 215, 3, 218, 214, 3, 222, 213, 4, 225, 212, 5, 229, 211, 5, 232, 211, 6, 234, 210, 6, 236, 209, 7, 239, 206, 8, 242, 203, 9, 244, 201, 10, 245, 199, 11, 247, 197, 12, 249, 191, 14, 251, 187, 15, 252, 183, 17, 253, 174, 19, 254, 168, 20, 254, 164, 21, 255, 161, 22, 255, 157, 23, 255, 149, 25, 255, 139, 28, 255, 131, 31, 255, 118, 34, 255, 104, 37, 255, 99, 39, 255, 88, 42, 254, 77, 45, 254, 62, 49, 253, 53, 52, 252, 45, 55, 251, 33, 59, 251, 31, 60, 251, 29, 61, 250, 27, 63, 249, 26, 66, 248, 25, 70, 247, 24, 75, 247, 25, 79, 247, 32, 83, 247, 38, 86, 247, 46, 90, 248, 55, 94, 248, 64, 98, 249, 81, 104, 250, 93, 108, 250, 98, 110, 251, 101, 112, 251, 109, 117, 252, 121, 123, 253, 130, 128, 254, 139, 133, 254, 151, 140, 255, 163, 146, 255, 173, 152, 255, 178, 155, 255, 191, 165, 255, 199, 172, 255, 207, 179, 255, 216, 185, 255, 220, 196, 255, 225, 202, 255, 230, 206};

void setup() {
  therm.begin(); // Initialize thermal IR sensor
  therm.setUnit(TEMP_C); // Set the library's units to Farenheit
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  int t;
  if (therm.read()){
    t = int(((therm.object()-0)/60.0)*127);
    for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(colormap[t*3],colormap[t*3+1],colormap[t*3+2])); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(delayval); // Delay for a period of time (in milliseconds).
    }
    //Serial.print(t);
    //Serial.print(" - ");
   // Serial.println(therm.object());
  }
  delay(100);
}
