#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  150
#define SERVOMAX  600

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup(){
    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop(){
    uint16_t p;
    p = map(45, 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(0, 0, p);
    delay(500);
    p = map(135, 0, 180, SERVOMIN, SERVOMAX);
    pwm.setPWM(0, 0, p);
    delay(500);
}

