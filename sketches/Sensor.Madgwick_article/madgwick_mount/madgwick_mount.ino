#include "I2Cdev.h"
#include "MPU6050.h"
#include "MadgwickAHRS.h"
#include <Servo.h>

#define TO_RAD 0.01745329252f
#define PITCH_DIR -1 // 1 - направление сервомотора и датчика совпадают, -1 - нет
#define PITCH_DEFAULT -90 // начальный угол датчика
#define ROLL_DIR -1
#define ROLL_DEFAULT 0
#define PITCH 0
#define ROLL 1

#define LPF 0.7


MPU6050 accelgyro;
Servo servos[2]; // тангаж (pitch), вращение (roll)
int angle[2] = {0,0}; // pitch, roll

unsigned long tm, imu_t, stab_t;
const unsigned int imu_to = 20; // каждые 20 мс опрашиваем датчик
const unsigned int stab_to = 50; // каждые 50 мс управляем сервомоторами
const unsigned int calib_to = 1000; // 1 секунду после старта калибруемся

float tdelta;
int16_t ax, ay, az;
int16_t gx_raw, gy_raw, gz_raw;

float imu[3];
float imu_f[3];
float quat[4];
//float e[3];

bool calibrated = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize(); // инициализация датчиков
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  servos[0].attach(2);
  servos[1].attach(3);
}

int clamp (int a, int mn, int mx){
  if( a>mx )
    return mx;
  if( a<mn )
    return mn;
  return a;
}

void loop() {
  // каждые 20мс вычисляем углы наклона
  tm = millis();
  if (imu_t + imu_to < tm) {
    tdelta = tm - imu_t; // вычисляем дельту времени в миллисекундах
    imu_t = tm;

    accelgyro.getMotion6(&ax, &ay, &az, &gx_raw, &gy_raw, &gz_raw);

    // преобразуем сырые данные гироскопа в рад/с
    float gx = gx_raw * TO_RAD / 131.0;
    float gy = gy_raw * TO_RAD / 131.0;
    float gz = gz_raw * TO_RAD / 131.0;    

    // вызываем алгоритм фильтра и передаем туда:
    // - дельту времени в секундах
    // - данные гироскопа в рад/с
    // - сырые данные акселерометра
    MadgwickAHRSupdateIMU(tdelta/1000.0, gx, gy, gz, (float)ax, (float)ay, (float)az);
    quat[0] = q0; quat[1] = q1; quat[2] = q2; quat[3] = q3;

    // преобразуем кватернион в углы Эйлера
    quat2Euler(&quat[0], &imu[0]);

    // применяем фильтр низких частот
    imu_f[0] = imu_f[0]*LPF + imu[0]*(1-LPF);
    imu_f[1] = imu_f[1]*LPF + imu[1]*(1-LPF);
    imu_f[2] = imu_f[2]*LPF + imu[2]*(1-LPF);
  }

  tm = millis();
  if (!calibrated && (calib_to < tm)) {
    calibrated = 1;
    imu_f[0] = imu[0];
    imu_f[1] = imu[1];
    imu_f[2] = imu[2];
  }

  // каждые 50мс управляем сервомоторами
  tm = millis();
  if (calibrated && (stab_t + stab_to < tm)) {
    stab_t = tm;
    float a = 0;
    // PITCH
    a = imu_f[0]/TO_RAD + PITCH_DEFAULT;
    if( abs(a)>2 ){
      angle[PITCH] = clamp(angle[PITCH] + a*PITCH_DIR, 0, 180);
      servos[PITCH].write(angle[PITCH]);
    }

    // ROLL
    a = imu_f[1]/TO_RAD + ROLL_DEFAULT;
    if( abs(a)>2 ){
      angle[ROLL] = clamp(angle[ROLL] + a*ROLL_DIR, 0, 180);
      servos[ROLL].write(angle[ROLL]);
    }
  }
}
