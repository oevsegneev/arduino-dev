#include <Arduino.h> 
#include <AccelStepper.h> 

#define RPM_1 3200//6400 
#define ACC_1 3200//6400 
#define MOT_STEPS_1 3200//6400 

#define M0 0 

AccelStepper *mtr[1]; 

const double rel[1] = {66.6666}; 

//const byte mtrPins[3][2] = {{9,3}}; // step, dir 
//const byte mtrEnPin = 5; 
const byte mtrPins[3][2] = {{2,5}}; // step, dir 
const byte mtrEnPin = 8; 

byte trigger = 0; 

void setup() { 
Serial.begin(9600); 

mtr[M0] = new AccelStepper(AccelStepper::DRIVER, mtrPins[M0][0], mtrPins[M0][1]); 

pinMode(mtrEnPin, OUTPUT); 
pinMode(A0, INPUT); 
pinMode(13, OUTPUT); 

mtr[M0]->setMaxSpeed(RPM_1); 
mtr[M0]->setAcceleration(ACC_1); 

digitalWrite(mtrEnPin, LOW); 

delay(1000); 
} 

void loop() { 
  int v = 1;//digitalRead(A0);
  if( v ){
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  if( !trigger && v ){ 
    mtr[0]->move(1000); 
    trigger = 1; 
  } 
  if( trigger && !v ){ 
    mtr[0]->move(-1000); 
    trigger = 0; 
  } 
  mtr[0]->run(); 
}
