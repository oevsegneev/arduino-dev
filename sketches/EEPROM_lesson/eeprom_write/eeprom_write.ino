#include <EEPROM.h>

const byte btnPin = 2;
byte counter = 0;
int addr = 0;

void setup() {
    Serial.begin(9600);
    pinMode(btnPin, INPUT);

    byte v = EEPROM.read(addr);
    Serial.print("saved counter = ");
    Serial.println(v);
}

void loop() {
    if( digitalRead(btnPin) == HIGH ){
        counter++;

        EEPROM.write(addr, counter);

        Serial.print("counter = ");
        Serial.println(counter);
        delay(200);
    }
}
