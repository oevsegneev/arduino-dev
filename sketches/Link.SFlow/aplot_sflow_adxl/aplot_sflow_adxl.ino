#include <ADXL345.h>

ADXL345 accelerometer;

Vector araw;

void setup() {
    Serial.begin(115200);
    
    accelerometer.begin();
    accelerometer.setRange(ADXL345_RANGE_16G);
}

void loop() {
    araw = accelerometer.readRaw();
    Serial.println( araw.XAxis );
    delay(20);
}

