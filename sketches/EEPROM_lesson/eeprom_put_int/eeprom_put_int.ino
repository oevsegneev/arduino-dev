#include <EEPROM.h>

const byte btnPin = 2;
const byte potPin = A0;
byte potIdx = 0;

void setup() {
    Serial.begin(9600);
    pinMode(btnPin, INPUT);
    pinMode(potPin, INPUT);

    // считываем значения из EEPROM в цикле
    // сначала по адресу 0, потом 2, потом 4
    // i - интератор, он же номер потенциометра
    Serial.println("Read data from EEPROM");
    for( byte i=0; i<3; i++ ){
        int v;
        EEPROM.get(i*2, v);
        Serial.print("value of pot #");
        Serial.print(i+1);
        Serial.print(" = ");
        Serial.println(v);
    }
    delay(3000);
}

void loop() {
    int v = analogRead(potPin);
    Serial.print("v = ");
    Serial.println(v);

    if( digitalRead(btnPin) == HIGH ){
        EEPROM.put(potIdx*2, v);

        Serial.println("Write data to EEPROM");
        Serial.print("value of pot #");
        Serial.print(potIdx+1);
        Serial.print(" = ");
        Serial.println(v);

        if( potIdx == 2 )
            potIdx = 0;
        else
            potIdx++;
        delay(1000);
    }
}
