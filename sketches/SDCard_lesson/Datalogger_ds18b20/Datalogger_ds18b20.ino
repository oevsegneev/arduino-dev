#include <SPI.h>
#include <SD.h>
#include <OneWire.h>

OneWire  ds(2);
File dataFile;

const int chipSelect = 4;

void setup() {
  pinMode(, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("Ready");
}

void loop() {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  float celsius;
  
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);

  delay(1000);

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
  if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
  else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
  else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms

  celsius = (float)raw / 16.0;
  Serial.print("t=");
  Serial.println(celsius);

  dataFile = SD.open("temp.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(celsius);
    dataFile.close();
  }
  else {
    Serial.println("error opening file");
  }
  //delay(500);
}
