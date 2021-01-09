#include "I2Cdev.h"
#include "MPU6050.h"

#define TO_DEG 57.29577951308232087679815481410517033f
#define T_OUT 20
#define FK 0.1

MPU6050 accelgyro;

float angle_ax, angle_gx, angle_cpl;
int dt = 0;
long int t_next;

float clamp(float v, float minv, float maxv){
    if( v>maxv )
        return maxv;
    else if( v<minv )
        return minv;
    return v;
}

void setup() {

  Serial.begin(9600);
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  long int t = millis();
  if( t_next < t ){
    int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
    float ay,gx;

    t_next = t + T_OUT;

    accelgyro.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
  
    gx = gx_raw / 16.4;
    ay = ay_raw / 4096.0;
    ay = clamp(ay, -1.0, 1.0);

    if( ay >= 0){
      angle_ax = 90 - TO_DEG*acos(ay);
    } else {
      angle_ax = TO_DEG*acos(-ay) - 90;
    }
    angle_gx = angle_gx + gx * T_OUT/1000.0;

    angle_gx = angle_gx*(1-FK) + angle_ax*FK;
    
    Serial.print(angle_gx);
    Serial.print(", ");
    Serial.print(angle_ax);
    Serial.println();
  }
}
