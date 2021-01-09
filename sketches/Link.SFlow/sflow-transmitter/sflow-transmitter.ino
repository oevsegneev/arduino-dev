/*
 Copyright (C) 2016 O. Evsegneev <oleg.evsegneev@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for sensor data transmitting. 
 */
#include <SerialFlow.h>

SerialFlow rd(&Serial);

const byte temp_pin = A0; 
const unsigned long data_to = 100; 
unsigned long tm, data_next; 

void setup(void){
    pinMode( temp_pin, INPUT);

    rd.setPacketFormat(2, 1, 0); // размер чисел - 2, размер пакета - 1
}

void loop(void){
    unsigned int v;
    tm = millis();
    if( tm > data_next ){
        data_next = tm + data_to;
        v = analogRead( temp_pin );
        rd.setPacketValue( v );
        rd.sendPacket();
    }
}

