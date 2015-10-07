static const byte PACKET_SIZE = 128;
static const byte VALUE_SIZE = 1;
static const boolean SEPARATE_VALUES = false;

#include <SerialFlow.h>
#include <math.h>

SerialFlow rd(&Serial); // RX, TX

const byte sz = 128;

void setup() {
    rd.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
    rd.begin(115200);
}

void loop() {
  for(int i=0; i<sz; i++){
    rd.setPacketValue(random(256));
  }
  rd.sendPacket();
  delay(100);
}

