#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;
  
void setup() {
  Serial.begin(9600);
  
  if (!bmp.begin()) {  
    Serial.println("Error!");
    while (1);
  }
}
  
void loop() {
    Serial.print("TEMP = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("PRES = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print("ALT = ");
    Serial.print(bmp.readAltitude(1013.25));
    Serial.println(" m");
    
    Serial.println();
    delay(2000);
}
