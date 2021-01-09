#include <SPI.h>
#include "nRF24L01.h"

uint8_t ce_pin = 7;
uint8_t csn_pin = 8;

void writeSingleReg( uint8_t addr, uint8_t value ){
    digitalWrite(csn_pin, LOW);
    SPI.transfer( W_REGISTER | (REGISTER_MASK & addr ) );
    SPI.transfer( value );
    digitalWrite(csn_pin, HIGH);
}

uint8_t readRegister(uint8_t reg){
    uint8_t result;
  
    digitalWrite(csn_pin, LOW);
    SPI.transfer( R_REGISTER | ( REGISTER_MASK & reg ) );
    result = SPI.transfer(0xff);
    digitalWrite(csn_pin, HIGH);

    return result;
}

void setup() {
    Serial.begin(115200);
    pinMode(ce_pin,OUTPUT);  
    pinMode(csn_pin,OUTPUT);
    
    SPI.begin();
    //digitalWrite(ce_pin, LOW);
    //digitalWrite(csn_pin, HIGH);
    //delay(5);
    //digitalWrite(ce_pin, HIGH);

    writeSingleReg( NRF_CONFIG, ( 1<<EN_CRC ) | ( 1<<PWR_UP ) );
    Serial.println( readRegister( NRF_CONFIG ));
}

void loop() {
  // put your main code here, to run repeatedly:

}
