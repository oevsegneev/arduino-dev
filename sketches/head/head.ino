#include "SoftwareServos.h"

#define RADIO 0

#if RADIO
#include <SerialFlowHW.h>
SerialFlowHW rd;
#endif

#define RADIO 0

#define S_LV 0
#define S_LH 1
#define S_LLIDS 2
#define S_LLIPS 3
#define S_LBROW 4
#define S_RV 5
#define S_RH 6
#define S_RLIDS 7
#define S_RLIPS 8
#define S_RBROW 9
#define S_MOUTH 10
#define S_PITCH 11
#define S_YAW 12

int servo[13] = {
2, //lv
11, //lh
4, //llids
10, //llips
8, //lbrow
3, //rv
5, //rh
6, //rlids
9, //rlips
7, //rbrow
12, //jaw
A1, //pitch
A0 //yaw
};
 
int zero[13] = {
115, // lv
65, // lh
90, // llids
90, // llips
90, // lbrow
90, // rv
90, // rh
85, // rlids
90, // rlips
90, // rbrow
90, // mouth
90, // pitch
90 // yaw
};

char bt;
int ctrl_time, detect_time, time;
int CTRL_TIMEOUT = 50;
int DETECT_TIMEOUT = 10000;
int led = 0;
int coord_x = 0;
int coord_y = 0;
int user_id = 0;
int user_ids[4]={14, 15, 16, 17};

float state_x = 0;
float state_y = 0;

boolean need_refresh = true;

int clamp( int val, int min_val, int max_val ){
    if( val < min_val )
        return min_val;
    if( val > max_val )
        return max_val;
    return val;
}

void setPosition( int num, int angle ){
    int a;
    a = (angle - 90)*1000/90.0;
    ss_SetPosition( servo[num], 1500 + a );
}

void setZPosition( int num, int inc ){
    setPosition( num, zero[num] + inc );
}

void offServo( int num ){
    int a;
    ss_SetPosition( servo[num], 0 );
}

void seeking_left(){
    for(int a=0; a>=-40; a-=5){
        setZPosition( S_LH, a );
        setZPosition( S_RH, a );
        delay(40);
    }
    delay(1000);
    for(int a=0; a<=40; a+=5){
        setZPosition( S_LH, -40+a );
        setZPosition( S_RH, -40+a );
        delay(40);
    }
    delay(1000);
    offServo(S_LH);
    offServo(S_RH);
}

void seeking_right(){
    for(int a=0; a<=40; a+=5){
        setZPosition( S_LH, a );
        setZPosition( S_RH, a );
        delay(40);
    }
    delay(1000);
    for(int a=0; a>=-40; a-=5){
        setZPosition( S_LH, 40+a );
        setZPosition( S_RH, 40+a );
        delay(40);
    }
    delay(1000);
    offServo(S_LH);
    offServo(S_RH);
}

void seeking_down(){
    for(int a=0; a<=30; a+=5){
        setZPosition( S_LV, a );
        setZPosition( S_RV, -a );
        delay(50);
    }
    delay(1000);
    for(int a=0; a>=-30; a-=5){
        setZPosition( S_LV, +30+a );
        setZPosition( S_RV, -30-a );
        delay(50);
    }
    delay(1000);
    offServo(S_LV);
    offServo(S_RV);
}

void blinking(){
    for(int a=0; a>=-50; a-=5){
        setZPosition( S_LLIDS, +a );
        setZPosition( S_RLIDS, +a );
        delay(20);
    }
    for(int a=0; a<=40; a+=5){
        setZPosition( S_LLIDS, -50+a );
        setZPosition( S_RLIDS, -50+a );
        delay(20);
    }
    delay(1000);
    offServo(S_LLIDS);
    offServo(S_RLIDS);
}

void smilingSad(){
    for (int a=0; a<=80; a+=5){
        setZPosition( S_LLIPS, -a );
        setZPosition( S_RLIPS, +a );
        delay(30);
    }
    delay(500);
    for (int a=80; a>=0; a-=5){
        setZPosition( S_LLIPS, -a );
        setZPosition( S_RLIPS, +a );
        delay(30);
    }
    delay(500);
    offServo(S_LLIPS);
    offServo(S_RLIPS);

}

void smilingHappy(){
  for(int a=0; a<=80; a+=5){
    setZPosition( S_LLIPS, +a );
    setZPosition( S_RLIPS, -a );
    delay(30);
  }
  delay(500);
  for(int a=80; a>=0; a-=5){
    setZPosition( S_LLIPS, +a );
    setZPosition( S_RLIPS, -a );
    delay(30);
  }
  delay(500); 
  offServo(S_LLIPS);
  offServo(S_RLIPS);
}

void blinking_one(){
    for(int a=0; a>=-50; a-=5){
        setZPosition( S_RLIDS, +a);
        delay(10);
    }
    for(int a=0; a<=50; a+=5){
        setZPosition( S_RLIDS, -50+a);
        delay(10);
    }
    delay(2000);
    offServo(S_RLIDS);
    offServo(S_RLIDS);
}

void talking(){
    for( int i=0; i<3; i++){
        setZPosition( S_MOUTH, -30);
        delay(200);
        setZPosition( S_MOUTH, 0);
        delay(200);
    }
    delay(1000);
    offServo(S_MOUTH);
}

void blinking_down(){
    //head_relax_y();
    for(int a=0; a<=30; a+=2){
        setZPosition( S_PITCH, +a );
        delay(50);
    }
    delay(1000);
    blinking();
    delay(1000);
    for(int a=0; a>=-30; a-=2){
        setZPosition( S_PITCH, +20+a );
        delay(50);
    }    
    delay(1000);
    offServo(S_PITCH);
    offServo(S_PITCH);
}

void eye_concentrate(){
    setZPosition( S_LLIDS, -30 );
    setZPosition( S_RLIDS, -30 );
    delay(500);
    offServo(S_LLIDS);
    offServo(S_RLIDS);
}

void eye_relax(){
    setZPosition( S_LLIDS, 0 );
    setZPosition( S_RLIDS, 0 );
    delay(500);
    offServo(S_LLIDS);
    offServo(S_RLIDS);
}

void observing(){
    //head_relax_x();
    setZPosition( S_LLIDS, -30 );
    setZPosition( S_RLIDS, -30 );
    for(int a=0; a<=20; a+=1){
        setZPosition( S_YAW, +a);
        delay(50);
    }
    delay(2000);
    for(int a=0; a>=-40; a-=1){
        setZPosition( S_YAW, +20+a );
        delay(50);
    }    
    delay(2000);
    for(int a=0; a<=20; a+=1){
        setZPosition( S_YAW, -20+a );
        delay(50);
    }
    setZPosition( S_LLIDS, 0 );
    setZPosition( S_RLIDS, 0 );
    delay(1000);
    offServo(S_LLIDS);
    offServo(S_RLIDS);
    offServo(S_YAW);
}

void setup() {
    #if RADIO
    rd.setPacketFormat( 2, 3 );
    rd.begin( 9600 );
    #endif
    //Serial.begin(9600);

    ss_Init();

    for( int i=0; i<13; i++ )
        setZPosition( i, 0 );
    
    state_x = zero[11];
    state_y = zero[12];
}

#if RADIO
void detect(){
    bool flag = false; 
    while( rd.receivePacket() ){
        coord_x = rd.getPacket(0) - 50;  
        coord_y = rd.getPacket(1) - 50;
        user_id = rd.getPacket(2);  
        led = !led;
        digitalWrite(13, led);
        need_refresh = true;
        for(int i=0; i<4; i++)
        {
          if (user_id == user_ids[i])
            flag= true;
        }
    }
    time = millis();
    if( need_refresh && (time - ctrl_time > CTRL_TIMEOUT) ){
        ctrl_time = time;
        //move_head();
        if (flag==true){
          //giveMeHappySmile();
        }
        else{
          //giveMeSadSmile();
        }
        need_refresh = false;
    }
}
#endif

void random_idle(){
    int action;
    action = random(8);
    switch (action){
//        case 0: 
//            observing();
//            break;
        case 0: 
        case 1: 
            blinking();
            break;
        case 2: 
            talking();
            break;
        case 3: 
            seeking_left();
            break;
        case 4: 
            seeking_right();
            break;
//        case 8: 
//            blinking_down();
//            break;
        case 5: 
            blinking_one();
            break;
        case 6:
              smilingHappy();
              break;
        case 7:
              smilingSad();
              break;
//        case 8:
//            detect_time = millis();
//            eye_concentrate();
//            break;
    }
}

void loop() {
    #if RADIO
    if( detect_time ){
        detect();
        time = millis();
        if( time - detect_time > DETECT_TIMEOUT ){
            detect_time = 0;
            eye_relax();
            delay(1000);
        } else
            return;
    }
    #endif
    
    random_idle();
    //blinking_down();
    //delay(1000);
} 
