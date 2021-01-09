#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  pinMode( 3, OUTPUT );
  mySwitch.enableReceive(0);
}

void loop() {
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    
    //if( value & B0001 )
    //  digitalWrite( 3, HIGH );
    //if( value & B0010 )
    //  digitalWrite( 3, LOW );

    Serial.println( value );
    mySwitch.resetAvailable();
  }
}
