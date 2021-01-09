#include <Servo.h>

Servo myservo;

const byte photoPin = A0;
const byte servoPin = 2;

void setup() {
  myservo.attach(servoPin);
}

void loop() {
  int val = analogRead(photoPin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map((1023-val)*2.5, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}

