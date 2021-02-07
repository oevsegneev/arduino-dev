static const byte PACKET_SIZE = 3;
static const byte VALUE_SIZE = 2;
static const boolean SEPARATE_VALUES = false;

#include <SerialFlow.h>

#include <Wire.h>
#include <ADXL345.h>
#include <HMC5883L.h>
#include <L3G.h>
#include "Madgwick.h"

#define TO_RAD 0.01745329252f
#define TO_DEG 57.29577951308f

const float mag_soft[3] = {-10.784294, -227.761480, -167.705562};
const float mag_hard[3][3] = {{1.256295, 0.005707, 0.018965},
                              {0.005707,1.174722,-0.000750},
                              {0.018965,-0.000750,1.321677}};

ADXL345 accelerometer;
HMC5883L compass;
L3G gyro;

SerialFlow rd(&Serial); // RX, TX

float imu[3], quat[4], mag[3];
uint32_t t, rt, st, mt;
const uint16_t mto = 15;
const uint16_t rto = 8;
const uint16_t sto = 50;

Vector mraw, araw;

void quat_2_euler( float q[4], float e[3] ){
   //double sqw = q[0]*q[0];
   double sqx = q[1]*q[1];
   double sqy = q[2]*q[2];
   double sqz = q[3]*q[3];

   e[0] = atan2f(2.f * (q[2]*q[3] + q[1]*q[0]), 1 - 2.f * (sqx + sqy));// -sqx - sqy + sqz + sqw);
   e[1] = asinf(-2.f * (q[1]*q[3] - q[2]*q[0]));
   e[2] = atan2f(2.f * (q[1]*q[2] + q[3]*q[0]), 1 - 2.f * (sqy + sqz));//sqx - sqy - sqz + sqw);
}

void setup() {
    rd.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
    rd.begin(115200);
    
    accelerometer.begin();
    accelerometer.setRange(ADXL345_RANGE_16G);

    compass.begin();
    compass.setRange(HMC5883L_RANGE_1_3GA);
    compass.setMeasurementMode(HMC5883L_CONTINOUS);
    compass.setDataRate(HMC5883L_DATARATE_75HZ);
    compass.setSamples(HMC5883L_SAMPLES_2);
    
    Wire.begin();
    gyro.init();
    gyro.enableDefault();
}

void loop() {
  float g[3];

  t = millis();
  if( t > mt + mto ){
    mt = t;
    mraw = compass.readRaw();
    mag[0] = mraw.XAxis - mag_soft[0];
    mag[1] = mraw.YAxis - mag_soft[1];
    mag[2] = mraw.ZAxis - mag_soft[2];

    mag[0] = mag_hard[0][0]*mag[0] + mag_hard[0][1]*mag[1] + mag_hard[0][2]*mag[2];
    mag[1] = mag_hard[1][0]*mag[0] + mag_hard[1][1]*mag[1] + mag_hard[1][2]*mag[2];
    mag[2] = mag_hard[2][0]*mag[0] + mag_hard[2][1]*mag[1] + mag_hard[2][2]*mag[2];    
  }
  
  t = millis();
  if( t > rt + rto ){
    rt = t;
  
    araw = accelerometer.readRaw();
    gyro.read();
    g[0] = TO_RAD*gyro.g.x*0.07;//2000.0/32768.0;
    g[1] = TO_RAD*gyro.g.y*0.07;//2000.0/32768.0;
    g[2] = TO_RAD*gyro.g.z*0.07;//2000.0/32768.0;

    MadgwickAHRSupdate( float(rto)/1000.0, g[0], g[1], g[2], 
                        float(araw.XAxis), float(araw.YAxis), float(araw.ZAxis), 
                        float(mag[0]), float(mag[1]), float(mag[2]));

    //MadgwickAHRSupdateIMU( float(rto)/1000.0, g[0], g[1], g[2], 
    //                      float(araw.XAxis), float(araw.YAxis), float(araw.ZAxis));

    quat[0] = q0; quat[1] = q1; quat[2] = q2; quat[3] = q3;
    quat_2_euler(&quat[0], &imu[0]);
  }

  t = millis();
  if( t > st + sto ){
    st = t;
    rd.setPacketValue(int(TO_DEG*imu[0]));
    rd.setPacketValue(int(TO_DEG*imu[1]));
    rd.setPacketValue(int(TO_DEG*imu[2]));
    rd.sendPacket();
  }
}

