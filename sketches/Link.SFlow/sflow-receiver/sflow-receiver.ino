/*
 Copyright (C) 2016 O. Evsegneev <oleg.evsegneev@gmail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for sensor data receiving. 
 */
#include <SerialFlow.h>

const byte ledPin = 13;

SerialFlow rd(&Serial);

void setup(void){
    pinMode(ledPin, OUTPUT);
    Serial.begin(57600);
    rd.setPacketFormat(2, 1, 0); // размер чисел - 2, размер пакета - 1
}

void loop(void){
    unsigned int v;
    if( rd.receivePacket() ){
        v = rd.getPacketValue(0);
        digitalWrite(ledPin, v);
    }
}
