#include "I2Cdev.h"
#include "MPU6050.h"

#define TO_DEG 57.29577951308232087679815481410517033f
#define T_OUT 20
#define FK 0.5

MPU6050 accelgyro;

float ay,ayf;
int dt = 0;
long int t_next;

void setup() {
  Serial.begin(9600);
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  long int t = millis();
  if( t_next < t ){
    int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
    float ay;

    accelgyro.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
  
    ay = ay_raw / 4096.0;
    ayf = ayf*(1-FK) + ay*FK;

 
    Serial.print(ay);
    Serial.print(", ");
    Serial.print(ayf);
    Serial.println();
  }
}
