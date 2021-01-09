static const byte PACKET_SIZE = 2;
static const byte VALUE_SIZE = 2;
static const boolean SEPARATE_VALUES = true;

#include <SerialFlow.h>
#include <math.h>

SerialFlow rd(&Serial); // RX, TX

const byte sz = 2;
int x = 0;

void setup() {
    rd.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
    rd.begin(115200);
}

void loop() {
  rd.setPacketValue( x );
  rd.setPacketValue( int( sin( x*3.14/180.0)*180) );
  rd.sendPacket();
  x++;
  if( x == 360 )
    x = 0;
  delay(20);
}

