#include <SerialFlow.h>
#include <ADXL345.h>

ADXL345 accelerometer;
SerialFlow rd(&Serial); // RX, TX

Vector araw;

void setup() {
    //Serial.begin(115200);
    rd.setPacketFormat(2, 3, 0);
    rd.begin(115200);
    
    accelerometer.begin();
    accelerometer.setRange(ADXL345_RANGE_16G);
}

void loop() {
    araw = accelerometer.readRaw();
    rd.setPacketValue( araw.XAxis + 58 );
    rd.setPacketValue( araw.YAxis - 30  );
    rd.setPacketValue( araw.ZAxis - 1420 );
    rd.sendPacket();
    //Serial.println( araw.YAxis );
    delay(20);
}

