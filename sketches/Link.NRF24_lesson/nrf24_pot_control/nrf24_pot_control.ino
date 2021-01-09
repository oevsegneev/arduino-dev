#include <SerialFlow.h>

SerialFlow rd(7,8);

const byte jx = A0;
const byte jy = A1;

const unsigned long data_to = 100; 
unsigned long tm, data_next;

void setup(void){
    rd.setPacketFormat(1, 1);
    rd.begin(0xF0F0F0F0E1LL,0xF0F0F0F0D2LL);

    pinMode(jx, INPUT);
    pinMode(jy, INPUT);
}

void loop(void){
    tm = millis();
    if( tm > data_next ){
        data_next = tm + data_to;
        int x = analogRead(jx);
        int y = analogRead(jy);
        char v;
        if( x<100 )
          v = 'B';
        else if( x>900 )
          v = 'F';

        if( y<100 )
          v = 'R';
        else if( y>900 )
          v = 'L';

        rd.setPacketValue( v );
        rd.sendPacket();
    }
}
