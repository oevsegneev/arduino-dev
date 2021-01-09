/*
 Copyright (C) 2016 O. Evsegneev <oleg.evsegneev@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Пример передачи данных через радиомодуль nRF24L01
 * Приемник
 * 
 * Example for sensor data transmitting with nRF24L01 radios. 
 * Receiver
 */
#include <SerialFlow.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
SerialFlow rd(7,8);

const byte temp_pin = A0; 
const unsigned long data_to = 100; 
unsigned long tm, data_next; 

void setup(void){
    pinMode( temp_pin, INPUT);

    rd.setPacketFormat(2, 1, 0);
    rd.begin(0xF0F0F0F0D2LL,0xF0F0F0F0E1LL);

    lcd.init();
    lcd.backlight();
    lcd.print("Hello");
}

void loop(void){
    unsigned int v;
    if( rd.receivePacket() ){
        v = rd.getPacketValue(0);
        lcd.setCursor(0,0);
        lcd.print(v);
    }
}
