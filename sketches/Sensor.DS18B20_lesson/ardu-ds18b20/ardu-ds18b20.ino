#include <OneWire.h>

OneWire  ds(2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  float celsius;
  
  if ( !ds.search(addr)) {
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
  ds.write(0xBE);

  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
  if (cfg == 0x00) raw = raw & ~7;  // точность 9-разрядов, 93,75 мс
  else if (cfg == 0x20) raw = raw & ~3; // точность 10-разрядов, 187,5 мс
  else if (cfg == 0x40) raw = raw & ~1; // точность 11-разрядов, 375 мс

  celsius = (float)raw / 16.0;
  Serial.print("t=");
  Serial.println(celsius);
}
