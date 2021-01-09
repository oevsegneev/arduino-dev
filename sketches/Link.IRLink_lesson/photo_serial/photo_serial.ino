#include <SoftwareSerial.h>

SoftwareSerial Photo (10, 11, 1);

void setup() {
  Serial.begin(9600);
  Photo.begin(4800);
}

void loop() {
  while( Photo.available() ){
    char v = Photo.read();
    Serial.write(v);
  }
}
