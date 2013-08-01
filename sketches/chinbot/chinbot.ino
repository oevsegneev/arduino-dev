#define IRpin_PIN      PINB
#define IRpin          3

#define MAXPULSE 65000
#define NUMPULSES 50
#define RESOLUTION 20 
#define FUZZINESS 20

uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing

#include "ircodes.h"

int mot_1en = 10;
int mot_11 = 8;
int mot_12 = 9;

int mot_2en = 5;
int mot_21 = 7;
int mot_22 = 6;

void setup(void) {
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
  
  pinMode(mot_1en, OUTPUT);
  pinMode(mot_11, OUTPUT);
  pinMode(mot_12, OUTPUT);

  pinMode(mot_2en, OUTPUT);
  pinMode(mot_21, OUTPUT);
  pinMode(mot_22, OUTPUT);
}

void loop(void) {
  int numberpulses;
  
  numberpulses = listenForIR();
  
  Serial.print("Heard ");
  Serial.print(numberpulses);
  Serial.println("-pulse long IR signal");
  if (IRcompare(numberpulses, ir_forward,sizeof(ir_forward)/4)) {
//    Serial.println("FWD");
    moveFwd();
  }
  if (IRcompare(numberpulses, ir_backward,sizeof(ir_backward)/4)) {
    moveBwd();
  }
  if (IRcompare(numberpulses, ir_left,sizeof(ir_left)/4)) {
    moveLeft();
  }
  if (IRcompare(numberpulses, ir_right,sizeof(ir_right)/4)) {
    moveRight();
  }
  if (IRcompare(numberpulses, ir_ok,sizeof(ir_ok)/4)) {
//    Serial.println("OK");
    moveStop();
  }
  delay(500);
}

//KGO: added size of compare sample. Only compare the minimum of the two
boolean IRcompare(int numpulses, int Signal[], int refsize) {
  int count = min(numpulses,refsize);
  Serial.print("count set to: ");
  Serial.println(count);
  for (int i=0; i< count-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;
    
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
    } else {
      return false;
    }
    
    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {
    } else {
      return false;
    }
    
  }
  return true;
}

int listenForIR(void) {
  currentpulse = 0;
  
  while (1) {
    uint16_t highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length
  
//  while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
       highpulse++;
       delayMicroseconds(RESOLUTION);

       if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    pulses[currentpulse][0] = highpulse;
  
    while (! (IRpin_PIN & _BV(IRpin))) {
       lowpulse++;
       delayMicroseconds(RESOLUTION);
       if (((lowpulse >= MAXPULSE)  && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;
    currentpulse++;
  }
}

void moveBwd(){
  digitalWrite(mot_11, HIGH);
  digitalWrite(mot_12, LOW);
  analogWrite(mot_1en, 128);

  digitalWrite(mot_21, HIGH);
  digitalWrite(mot_22, LOW);
  analogWrite(mot_2en, 128);
}

void moveFwd(){
  digitalWrite(mot_11, LOW);
  digitalWrite(mot_12, HIGH);
  analogWrite(mot_1en, 128);

  digitalWrite(mot_21, LOW);
  digitalWrite(mot_22, HIGH);
  analogWrite(mot_2en, 128);
}

void moveRight(){
  digitalWrite(mot_11, LOW);
  digitalWrite(mot_12, HIGH);
  analogWrite(mot_1en, 128);

  digitalWrite(mot_21, LOW);
  digitalWrite(mot_22, HIGH);
  analogWrite(mot_2en, 0);
}

void moveLeft(){
  digitalWrite(mot_11, LOW);
  digitalWrite(mot_12, HIGH);
  analogWrite(mot_1en, 0);

  digitalWrite(mot_21, LOW);
  digitalWrite(mot_22, HIGH);
  analogWrite(mot_2en, 128);
}

void moveStop(){
  analogWrite(mot_1en, 0);
  analogWrite(mot_2en, 0);
}
