#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  mySwitch.enableTransmit(2);
}

void loop() {
  mySwitch.send(B0001, 4);
  delay(1000);  
  mySwitch.send(B0010, 4);
  delay(1000);  
}
