#include <Servo.h>

#define SERVO_MAX 2000
#define SERVO_MIN 1000
#define SERVO_D 1000

Servo ss;

void blip(byte count){
    for( byte c=0; c<count; c++ ){
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
    }
}

void setPwr( int p ){
//    ss.writeMicroseconds( SERVO_MIN + int(p*SERVO_D/100.0) );
    ss.writeMicroseconds( SERVO_MIN + int((p/100.0)*SERVO_D) );
}

void setup() {
    pinMode(13, OUTPUT);
    
    ss.attach(3);
    blip(1);
    
    // устанавливаем тягу в 0
    setPwr(0);

    // 5 секунд на то, чтобы подключить ESC к аккумулятору
    blip(5);

    // шагами повышаем скорость мотора
    for( int i=0; i<100; i+=5){
      setPwr(i);
      delay(300);
    }
    // шагами понижаем скорость мотора
    for( int i=100; i>=0; i-=5){
      setPwr(i);
      delay(300);
    }
}

void loop() {
    return;
}
