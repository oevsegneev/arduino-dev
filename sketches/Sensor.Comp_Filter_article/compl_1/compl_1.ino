#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

#define TO_DEG 57.29577951308232087679815481410517033f
#define T_OUT 20 // каждый 20 миллисекунд будем проводить вычисления 
#define P_OUT 50 // каждый 50 миллисекунд будем выводить данные
#define FK 0.1 // коэффициент комплементарного фильтра

MPU6050 accelgyro;

float angle_ax, angle_gx, angle_cpl;
int dt = 0;
long int t_next, p_next;

// функция, которая не даёт значению выйти за пределы 
float clamp(float v, float minv, float maxv){
    if( v>maxv )
        return maxv;
    else if( v<minv )
        return minv;
    return v;
}

void setup() {
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
  #endif

  Serial.begin(9600);
  // инициализация MPU6050
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  long int t = millis();
  // каждые T_TO миллисекунд выполняем рассчет угла наклона
  if( t_next < t ){
    int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
    float ay,gx;

    t_next = t + T_OUT;
    // получаем сырые данные от датчиков в MPU6050
    accelgyro.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);

    // преобразуем сырые данные гироскопа в град/сек
    gx = gx_raw / 16.4;
    // преобразуем сырые данные акселерометра в G
    ay = ay_raw / 4096.0;
    ay = clamp(ay, -1.0, 1.0);

    // вычисляем угол наклона по акселерометру
    angle_ax = 90 - TO_DEG*acos(ay);
    // вычисляем угол наклона по гироскопу
    angle_gx = angle_gx + gx * T_OUT/1000.0;
    // корректируем значение угла с помощью акселерометра
    angle_gx = angle_gx*(1-FK) + angle_ax*FK;
  }

  t = millis();
  // каждые P_OUT миллисекунд выводим результат в COM порт
  if( p_next < t ){
    p_next = t + P_OUT;
    Serial.println(angle_gx);
  }
}
