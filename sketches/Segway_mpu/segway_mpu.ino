#define betaDef 0.2f
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Madgwick.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
unsigned long tm, imu_t, prn_t;
unsigned int imu_to = 20;

const unsigned int prn_to = 100;

float tdelta;
int16_t ax, ay, az;
int16_t gx_raw, gy_raw, gz_raw;
MPU6050 accelgyro;
//#define OUTPUT_READABLE_ACCELGYRO
#define TO_RAD 0.01745329252f

bool blinkState = false;

float imu[3];
float quat[4];
float e[3];

void setup() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(9600);
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx_raw, &gy_raw, &gz_raw);
  float gx = gx_raw * TO_RAD / 16.4;
  float gy = gy_raw * TO_RAD / 16.4;
  float gz = gz_raw * TO_RAD / 16.4;
  tm = millis();
  if (imu_t + imu_to < tm) {
    tdelta = tm - imu_t;
    imu_t = tm;
    MadgwickAHRSupdateIMU(tdelta/1000.0, gx, gy, gz, (float)ax, (float)ay, (float)az);
    quat[0] = q0; quat[1] = q1; quat[2] = q2; quat[3] = q3;
    quat2Euler(&quat[0], &imu[0]);
  }
  tm = millis();
  if (prn_t + prn_to < tm) {
    prn_t = tm;
//    Serial.print("x|y|z\t");
//    Serial.print(ax); Serial.print("\t");
//    Serial.print(ay); Serial.print("\t");
//    Serial.println(az);
    Serial.print(imu[0]/TO_RAD); Serial.print("\t");
    Serial.print(imu[1]/TO_RAD); Serial.print("\t");
    Serial.println(imu[2]/TO_RAD);
//    Serial.print(gx); Serial.print("\t");
//    Serial.print(gy); Serial.print("\t");
//    Serial.println(gz);
  }
}
