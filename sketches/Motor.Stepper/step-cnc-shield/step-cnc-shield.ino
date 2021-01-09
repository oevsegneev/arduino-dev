#include <Arduino.h> 
#include <AccelStepper.h> 

#define RPM_1 3200//6400 
#define ACC_1 3200//6400 
#define MOT_STEPS_1 3200//6400 

#define M0 0 

AccelStepper *mtr[1]; 

const double rel[1] = {66.6666}; 

const byte mtrPins[3][2] = {{3,6}}; // step, dir 
const byte mtrEnPin = 8; 

byte trigger = 0; 

void setup() { 
Serial.begin(9600); 

mtr[M0] = new AccelStepper(AccelStepper::DRIVER, mtrPins[M0][0], mtrPins[M0][1]); 

pinMode(mtrEnPin, OUTPUT); 

mtr[M0]->setMaxSpeed(RPM_1); 
mtr[M0]->setAcceleration(ACC_1); 

digitalWrite(mtrEnPin, LOW); 

mtr[M0]->moveTo(500);

delay(1000);
} 

void loop() { 
    if (mtr[M0]->distanceToGo() == 0)
      mtr[M0]->moveTo(-mtr[M0]->currentPosition());

    mtr[M0]->run();
}
