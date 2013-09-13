#include <Servo.h>
#include <NewPing.h>

// set the names of the servos
/*
frh = front right hip
 frl = front right leg
 flh = front left hip
 fll = front left leg
 rlh = rear left hip
 rll = rear left leg
 rrh = rear right hip
 rrl = rear right leg
 eye = sevor for sensor
 */
Servo frh;
Servo frl;
Servo flh;
Servo fll;
Servo rlh;
Servo rll;
Servo rrh;
Servo rrl;
Servo eye;

NewPing sonar(4, 3, 300);

// set variables needed for some tasks

//set logical values for physical home position (cheap Chinese servos are cheap!)
int inc = 20;
int home_frh = 95;
int home_frl = 95+inc;
int home_flh = 95;
int home_fll = 90-inc;
int home_rlh = 90;
int home_rll = 95+inc;
int home_rrh = 90;
int home_rrl = 90-inc;

int b;
int x;
int w;
int up;
int up2;
int up3;
int down;
int down2;
int steps;
int rightsteps;
int leftsteps;
int back;
int pos;
int dist = 0;

int time;
int strafe_time = 0;

boolean strafe_right_flag = 0;
boolean strafe_left_flag = 0;

int buzz_pin = A0;
int led_pin = 13;

const int STRAFE_TIMEOUT = 3000;
//////////////////////////////////////
void setup()
{
  pinMode(buzz_pin, OUTPUT);  // LED pin
  pinMode(led_pin, OUTPUT);  // LED pin
  // assign servos to pins and center servos
  frh.attach(5);
  frh.write(home_frh);
  frl.attach(6);
  frl.write(home_frl);
  flh.attach(7);
  flh.write(home_flh);
  fll.attach(8);
  fll.write(home_fll);
  rlh.attach(9);
  rlh.write(home_rlh);
  rll.attach(10);
  rll.write(home_rll);
  rrh.attach(11);
  rrh.write(home_rrh);
  rrl.attach(12);
  rrl.write(home_rrl);
  
  delay(1000);
  //eyes
  //Serial.begin(9600);
  
  blink(10);
}

void blink( int count ){
    for( char i; i<count; i++ ){
        digitalWrite(led_pin, HIGH);
        digitalWrite(buzz_pin, HIGH);
        delay(100);
        digitalWrite(led_pin, LOW);
        digitalWrite(buzz_pin, LOW);
        delay(100);
    }
}

void idle(){
    delay(100);  // if set to a bigger number (more delay between the steps -> slower walking) you will see the walking pattern more clearly
}

void forward() {
    rll.write(home_rll+20);
    frl.write(home_frl+45);
    frh.write(home_frh+45);
    rrh.write(home_rrh-45);
    flh.write(home_flh);
    rlh.write(home_rlh);
    idle();
    rll.write(home_rll);
    frl.write(home_frl);
    idle();

    rrl.write(home_rrl-20);
    fll.write(home_fll-45);
    flh.write(home_flh-45);
    rlh.write(home_rlh+45);
    frh.write(home_frh);
    rrh.write(home_rrh);
    idle();
    rrl.write(home_rrl);
    fll.write(home_fll);
    //idle();
}

void strafe_right_setup(){
    rrl.write(home_rrl-20);
    rrh.write(home_rrh-60);
    idle();
    rrl.write(home_rrl);

    rll.write(home_rll+20);
    rlh.write(home_rlh+60);
    idle();
    rll.write(home_rll);

    frl.write(home_frl+20);
    flh.write(home_frh+45);
    idle();
    frl.write(home_frl);
}

void strafe_right(){
    rrl.write(home_rrl-45);
    fll.write(home_fll-20);
    rrh.write(home_rrh-45);
    flh.write(home_flh-55);

    rlh.write(home_rlh+45);
    frh.write(home_frh+55);
    idle();
    rrl.write(home_rrl);
    fll.write(home_fll);
    idle();

    frl.write(home_frl+45);
    rll.write(home_rll+20);
    frh.write(home_frh+45);
    rlh.write(home_rlh+55);

    flh.write(home_flh-45);
    rrh.write(home_rrh-55);
    idle();
    frl.write(home_frl);
    rll.write(home_rll);
    //idle();
}

void strafe_left(){
}

void loop(){
    dist = sonar.convert_cm( sonar.ping_median(5) );
    //Serial.println(dist);
    time = millis();
    
    if( dist > 10 && (time - strafe_time > STRAFE_TIMEOUT) ){
      strafe_right_flag = 0;
      strafe_left_flag = 0;
      forward();
    } else {
      if( !strafe_right_flag ){
          strafe_right_setup();
          strafe_right_flag = 1;
          strafe_time = millis();
      } else
          strafe_right();
    }  
}

