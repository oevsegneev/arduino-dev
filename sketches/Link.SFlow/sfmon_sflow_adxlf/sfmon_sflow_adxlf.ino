#include <SerialFlow.h>
#include <ADXL345.h>

ADXL345 accelerometer;
SerialFlow rd(&Serial); // RX, TX

Vector araw;

float fx,fy,fz;
const float k = 0.2;

void setup() {
    rd.setPacketFormat(2, 3, 0);
    rd.begin(115200);
    
    accelerometer.begin();
    accelerometer.setRange(ADXL345_RANGE_16G);

    fx = 0;
    fy = 0;
    fz = 0;
}

void loop() {
    araw = accelerometer.readRaw();

    fx = fx*(1-k) + (araw.XAxis + 58)*k;
    fy = fy*(1-k) + (araw.YAxis - 30)*k;
    fz = fz*(1-k) + (araw.ZAxis - 1420)*k;

    rd.setPacketValue( int(fx) );
    rd.setPacketValue( int(fy) );
    rd.setPacketValue( int(fz) );
    rd.sendPacket();
    delay(10);
}

