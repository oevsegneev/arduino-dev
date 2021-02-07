static const byte PACKET_SIZE = 3;
static const byte VALUE_SIZE = 2;
static const boolean SEPARATE_VALUES = false;

#include <SerialFlow.h>

#include <Wire.h>
#include <ADXL345.h>
#include <HMC5883L.h>
#include <L3G.h>
#include "Madgwick.h"
#include "utils.h"

#define TO_RAD 0.01745329252f
#define TO_DEG 57.29577951308f
#define ACC_GS 0.0383046875
#define ACC_DEADBAND 0.3
#define LPFC_ACC 0.1

#define X 0
#define Y 1
#define Z 2

const float mag_soft[3] = {-10.784294, -227.761480, -167.705562};
const float mag_hard[3][3] = {{1.256295, 0.005707, 0.018965},
                              {0.005707,1.174722,-0.000750},
                              {0.018965,-0.000750,1.321677}};

ADXL345 accelerometer;
HMC5883L compass;
L3G gyro;

SerialFlow rd(&Serial); // RX, TX

float imu[4], quat[4], mag[3], acc_lpf[3], grv_err[3], grv_old[3], grv[3];
float acc_1g;

uint32_t t, rt, st, mt;
const uint16_t mto = 15;
const uint16_t rto = 8;
const uint16_t sto = 50;

Vector mraw, araw;

void get_grv(float g[3]){
    g[X] = 2 * (quat[1] * quat[3] - quat[0] * quat[2]);
    g[Y] = 2 * (quat[0] * quat[1] + quat[2] * quat[3]);
    g[Z] = (quat[0] * quat[0] - quat[1] * quat[1] - quat[2] * quat[2] + quat[3] * quat[3]);
}

void get_alt( float tdelta ){
    float vdelta;
    float g[3];
    float tmp = 1.0f;
    
    // fetch gravitational vector from position quaternion
    get_grv(&g[0]);
    
    tmp = (acc_lpf[X]-grv_err[0]) * g[X] + (acc_lpf[Y]-grv_err[1]) * g[Y];

    grv_old[0] = grv[0];
    grv[0] = ((tmp + (acc_lpf[Z] - grv_err[2]) * g[Z]) * invSqrt(g[X]*g[X] + g[Y]*g[Y] + g[Z]*g[Z]) - acc_1g)* ACC_GS;
    
    // calc velocity in meters per seconds by median integration
    grv_old[1] = grv[1];
    grv[1] = grv[1] + deadband((grv[0]+grv_old[0])/2.0f, ACC_DEADBAND) * tdelta;

    // calc displacement in meters by median integration
    vdelta = (grv[1]+grv_old[1]) * tdelta / 2.0f;
    
    imu[3] += vdelta;
}

void calibrate_acc_grv(){
    int16_t data[3];
    int32_t avg[3] = {0,0,0};
    for(uint16_t i=0; i<100; i++ ){
        araw = accelerometer.readRaw();
        avg[X] += araw.XAxis;
        avg[Y] += araw.YAxis;
        avg[Z] += araw.ZAxis;
        delay(10);
    }
    
    // calc averages and set default values for lpf
    for(uint8_t i=0; i<3; i++ ){
        avg[i] = avg[i]/100;
        acc_lpf[i] = float(avg[i]);
    }
    acc_1g = 1.0f/invSqrt(avg[X]*avg[X] + avg[Y]*avg[Y] + avg[Z]*avg[Z]);
    
    grv_err[2] = avg[Z] - acc_1g;
}

void setup() {
    rd.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
    rd.begin(115200);
    //Serial.begin(115200);
    
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

    calibrate_acc_grv();
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

    acc_lpf[0] = acc_lpf[0]*(1-LPFC_ACC) + araw.XAxis*LPFC_ACC;
    acc_lpf[1] = acc_lpf[1]*(1-LPFC_ACC) + araw.YAxis*LPFC_ACC;
    acc_lpf[2] = acc_lpf[2]*(1-LPFC_ACC) + araw.ZAxis*LPFC_ACC;
    
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
    get_alt(float(rto)/1000.0);
  }

  t = millis();
  if( t > st + sto ){
    st = t;
    /*Serial.print(int(acc_1g));
    Serial.print("-");
    Serial.println(int(imu[3]*100));*/
    rd.setPacketValue(int(imu[3]));
    rd.setPacketValue(int(acc_lpf[0]));
    rd.setPacketValue(int(acc_lpf[1]));
    //rd.setPacketValue(int(TO_DEG*imu[0]));
    //rd.setPacketValue(int(TO_DEG*imu[1]));
    //rd.setPacketValue(int(TO_DEG*imu[2]));
    rd.sendPacket();
  }
}

