#define NORMAL 0
#define PAUSED 1

const float e1k = 1.;
const float e2k = 1.;

byte state = NORMAL;

char incomingByte;
byte cspeed = 0;

int pwm1pin = 5;
int pwm2pin = 6;
int dir1pin = 4;
int dir2pin = 7;

int buzz = 12;

int clr[3] = {10,11,9};

void beep(byte q, int t){
  for(int i=0; i<q; i++){
    digitalWrite(buzz, HIGH);
    delay(t);
    digitalWrite(buzz, LOW);
    delay(t);
  }
}

void setSpeed(byte spd){
  analogWrite(pwm1pin, int(spd*e1k));
  analogWrite(pwm2pin, int(spd*e2k));
}

void turnRight(){
  setSpeed(cspeed);
  digitalWrite(dir1pin, HIGH);
  digitalWrite(dir2pin, LOW);
}

void turnLeft(){
  setSpeed(cspeed);
  digitalWrite(dir1pin, LOW);
  digitalWrite(dir2pin, HIGH);
}

void backward(){
  setSpeed(cspeed);
  digitalWrite(dir1pin, LOW);
  digitalWrite(dir2pin, LOW);
}

void forward(){
  setSpeed(cspeed);
  digitalWrite(dir1pin, HIGH);
  digitalWrite(dir2pin, HIGH);
}

void mstop(){
  setSpeed(0);
}

void setup() {
    Serial.begin(9600);
    
    // init motors
    pinMode(pwm1pin, OUTPUT);
    pinMode(pwm2pin, OUTPUT);
    pinMode(dir1pin, OUTPUT);
    pinMode(dir2pin, OUTPUT);

    pinMode(buzz, OUTPUT);

    beep(3,200);
    beep(1,500);
}

void movement(char ib){
    switch(ib){
    case 'R':{
        turnRight();
        break;  
    }
    case 'L':{
        turnLeft();
        break;  
    }
    case 'F':{
        forward();
        break;  
    }
    case 'B':{
        backward();
        break;  
    }
    case 'D':{
        setSpeed(0);
    }
    case 'q':{
        setSpeed(255);
    }
    default:{
        mstop();
    }
    }
}

void loop() {  
    if( Serial.available() > 0 ){
        incomingByte = Serial.read(); 
        if(state==PAUSED)
            return;
        if( incomingByte>='0' && incomingByte<='9' ){
            cspeed = 100+(incomingByte-48)*15;
            //setSpeed(cspeed);
            return;
        }
        movement(incomingByte);
    }
}
