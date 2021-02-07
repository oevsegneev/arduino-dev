
#include <Servo.h>

Servo servos[13];

void setup()
{

}


void loop()
{
  servos[8].attach(8);
  servos[8].write((map(analogRead(A0), 0, 1024, 0, 180)));
  delay(50);

}
