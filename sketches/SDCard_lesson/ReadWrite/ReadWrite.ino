#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

void setup() {
  Serial.begin(9600);

  if( !SD.begin( chipSelect )){
    Serial.println("initialization failed!");
    return;
  }

  File myFile = SD.open("test.txt");
  if (myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
}


