/*
 Copyright (C) 2016 O. Evsegneev <oleg.evsegneev@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Пример передачи данных через радиомодуль nRF24L01
 * Передатчик
 * 
 * Example for sensor data transmitting with nRF24L01+ radios. 
 * Transmitter
 */
#include <SerialFlow.h>

SerialFlow rd(7,8);

const byte temp_pin = A0; 
const unsigned long data_to = 100; 
unsigned long tm, data_next; 

void setup(void){
    pinMode( temp_pin, INPUT);

    rd.setPacketFormat(2, 1, 0);
    rd.begin(0xF0F0F0F0E1LL,0xF0F0F0F0D2LL);
}

void loop(void){
    unsigned int v;
    tm = millis();
    if( tm > data_next ){
        data_next = tm + data_to;
        v = analogRead( temp_pin );
        rd.setPacketValue( ( v/1023.0 )*5.0*1000/10 );
        rd.sendPacket();
    }
}

