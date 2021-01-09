#include <Arduino.h>
#include "BasicStepperDriver.h"

#define MOTOR_STEPS 48

#define DIR 2
#define STEP 3

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

void setup() {
  stepper.setRPM(120);
  stepper.move(96);
  delay(2000);
  stepper.move(-96);
}

void loop() {
}
