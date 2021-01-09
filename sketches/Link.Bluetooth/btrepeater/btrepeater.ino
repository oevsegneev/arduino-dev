#include <SoftwareSerial.h>

int gRxPin = 10;
int gTxPin = 11;

int b1, b2, dist;

SoftwareSerial BTSerial(gRxPin, gTxPin);

void setup() {
  BTSerial.begin(57600);
  Serial.begin(9600);
  delay(500);
}

void loop() {
  BTSerial.write(0x55);
  if (BTSerial.available()) {
    b1 = BTSerial.read();
    b2 = BTSerial.read();
    dist = (b1 << 8) + b2;
    Serial.println(dist);
  }
  delay(100);
}
