#include <SerialFlow.h>

const int adc1Pin = A0;
const int adc2Pin = A1;

SerialFlow rd(&Serial);

void setup(){
    rd.setPacketFormat(2, 2, 0);
    rd.begin(115200);
}

void loop(){
    int val1, val2;
    val1 = analogRead(adc1Pin);
    val2 = analogRead(adc2Pin);
    rd.setPacketValue( val1/4 );
    rd.setPacketValue( ( val2/1023.0 )*500 );
    rd.sendPacket();
    delay(100);
}
