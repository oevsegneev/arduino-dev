/*
 Copyright (C) 2016 O. Evsegneev <oleg.evsegneev@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for sensor data transmitting with nRF24L01+ radios. 
 */
#include <SerialFlow.h>

SerialFlow rd(7,8);

const unsigned long data_to = 100; 
unsigned long tm, data_next; 

void setup(void){
    Serial.begin(57600);
    rd.setPacketFormat(2, 1, 0);
    rd.begin(0xF0F0F0F0D2LL,0xF0F0F0F0E1LL);
}

void loop(void){
    unsigned int v;
    if( rd.receivePacket() ){
        v = rd.getPacketValue(0);
        Serial.println(v);
    }
}
