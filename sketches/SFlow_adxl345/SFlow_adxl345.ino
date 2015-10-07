static const byte PACKET_SIZE = 2;
static const byte VALUE_SIZE = 2;
static const boolean SEPARATE_VALUES = true;

#include <SerialFlow.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
SerialFlow rd(&Serial); // RX, TX

float acc_lpf[2] = {0,0};

void setup() {
    rd.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
    rd.begin(115200);
    
    accel.begin();
    accel.setRange(ADXL345_RANGE_2_G);
}

void loop() {
    sensors_event_t event; 
    accel.getEvent(&event);

    acc_lpf[0] = acc_lpf[0]*0.98 + event.acceleration.x*0.02;
    acc_lpf[1] = acc_lpf[1]*0.98 + event.acceleration.y*0.02;
    
    rd.setPacketValue(int(acc_lpf[0]*10));
    rd.setPacketValue(int(acc_lpf[1]*10));
    rd.sendPacket();

    delay(1);
}

