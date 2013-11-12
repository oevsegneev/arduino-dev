#include <Servo.h> 
 
Servo srv_l_eye_v_ball;
Servo srv_l_eye_h_ball;
Servo srv_l_eye_lids;
//Servo srv_l_brow;
Servo srv_l_lips;

Servo srv_r_eye_v_ball;
Servo srv_r_eye_h_ball;
Servo srv_r_eye_lids;
//Servo srv_r_brow;
Servo srv_r_lips;

Servo srv_mouth;
Servo srv_yaw;
Servo srv_pitch;

int zero[13];

void set_zeros(){
    zero[0] = 90;  // lv
    zero[1] = 100; // lh
    zero[2] = 140; // llids
    zero[3] = 115; // lbrow
    zero[4] = 80; // llips
    zero[5] = 65; // rv
    zero[6] = 125; //rh
    zero[7] = 120; // rlids
    zero[8] = 85; // rbrow
    zero[9] = 90; // rlips
    zero[10] = 110;
    zero[11] = 90;
    zero[12] = 70;
}

void reset(){
    srv_l_eye_v_ball.write(zero[0]);
    srv_l_eye_h_ball.write(zero[1]);
    delay(500);
    srv_l_eye_lids.write(zero[2]);
    //srv_l_brow.write(zero[3]);
    srv_l_lips.write(zero[4]);
    delay(500);

    srv_r_eye_v_ball.write(zero[5]);
    srv_r_eye_h_ball.write(zero[6]);
    delay(500);
    srv_r_eye_lids.write(zero[7]);
    //srv_r_brow.write(zero[8]);
    srv_r_lips.write(zero[9]);
    delay(500);

    srv_mouth.write(zero[10]);
    delay(500);
    srv_yaw.write(zero[11]);
    delay(500);
    srv_pitch.write(zero[12]);
    delay(500);
}

void random_idle(){
    int action;
    action = random(10);
    switch (action){
        case 0: 
            observing();
            break;
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
        case 5: 
            blinking_left();
            break;
        case 6: 
            blinking_right();
            break;
        case 7: 
            blinking_down();
            break;
        case 8: 
            blinking_one();
            break;
        case 9:
            seeking_down();
            break;
    }
}

void seeking_left(){
    for(int a=0; a>=-40; a-=5){
        srv_l_eye_h_ball.write(zero[1]+a);
        srv_r_eye_h_ball.write(zero[6]+a);
        delay(40);
    }
    delay(1000);
    for(int a=0; a<=40; a+=5){
        srv_l_eye_h_ball.write(zero[1]-40+a);
        srv_r_eye_h_ball.write(zero[6]-40+a);
        delay(40);
    }
    delay(1000);
}

void seeking_right(){
    for(int a=0; a<=40; a+=5){
        srv_l_eye_h_ball.write(zero[1]+a);
        srv_r_eye_h_ball.write(zero[6]+a);
        delay(40);
    }
    delay(1000);
    for(int a=0; a>=-40; a-=5){
        srv_l_eye_h_ball.write(zero[1]+40+a);
        srv_r_eye_h_ball.write(zero[6]+40+a);
        delay(40);
    }
    delay(1000);
}

void seeking_down(){
    for(int a=0; a<=30; a+=5){
        srv_l_eye_v_ball.write(zero[0]+a);
        srv_r_eye_v_ball.write(zero[5]-a);
        delay(50);
    }
    delay(1000);
    for(int a=0; a>=-30; a-=5){
        srv_l_eye_v_ball.write(zero[0]+30+a);
        srv_r_eye_v_ball.write(zero[5]-30-a);
        delay(50);
    }
    delay(1000);
}

void blinking(){
    for(int a=0; a>=-50; a-=5){
        srv_l_eye_lids.write(zero[2]+a);
        srv_r_eye_lids.write(zero[7]+a);
        delay(10);
    }
    for(int a=0; a<=50; a+=5){
        srv_l_eye_lids.write(zero[2]-50+a);
        srv_r_eye_lids.write(zero[7]-50+a);
        delay(10);
    }
    delay(1000);
}

void blinking_one(){
    for(int a=0; a>=-50; a-=5){
        srv_r_eye_lids.write(zero[7]+a);
        delay(10);
    }
    for(int a=0; a<=50; a+=5){
        srv_r_eye_lids.write(zero[7]-50+a);
        delay(10);
    }
    delay(2000);
}

void sleeping(){
    for(int a=0; a>=-50; a-=5){
        srv_l_eye_lids.write(zero[2]+a);
        srv_r_eye_lids.write(zero[7]+a);
        delay(10);
    }
    delay(5000);
    for(int a=0; a<=50; a+=5){
        srv_l_eye_lids.write(zero[2]-50+a);
        srv_r_eye_lids.write(zero[7]-50+a);
        delay(10);
    }
    delay(1000);
}

void talking(){
    for( int i=0; i<3; i++){
        srv_mouth.write(zero[10]-30);
        delay(200);
        srv_mouth.write(zero[10]);
        delay(200);
    }
    delay(1000);
}

void blinking_left(){
    for(int a=0; a<=20; a+=1){
        srv_yaw.write(zero[11]+a);
        delay(50);
    }
    delay(1000);
    blinking();
    delay(1000);
    for(int a=0; a>=-20; a-=1){
        srv_yaw.write(zero[11]+20+a);
        delay(50);
    }    
    delay(1000);
}

void blinking_right(){
    for(int a=0; a>=-20; a-=1){
        srv_yaw.write(zero[11]+a);
        delay(50);
    }
    delay(1000);
    blinking();
    delay(1000);
    for(int a=0; a<=20; a+=1){
        srv_yaw.write(zero[11]-20+a);
        delay(50);
    }    
    delay(1000);
}

void blinking_down(){
    for(int a=0; a<=30; a+=2){
        srv_pitch.write(zero[12]+a);
        delay(50);
    }
    delay(1000);
    blinking();
    delay(1000);
    for(int a=0; a>=-30; a-=2){
        srv_pitch.write(zero[12]+20+a);
        delay(50);
    }    
    delay(1000);
}

void observing(){
    srv_l_eye_lids.write(zero[2]-30);
    srv_r_eye_lids.write(zero[7]-30);
    for(int a=0; a<=20; a+=1){
        srv_yaw.write(zero[11]+a);
        delay(50);
    }
    delay(2000);
    for(int a=0; a>=-40; a-=1){
        srv_yaw.write(zero[11]+20+a);
        delay(50);
    }    
    delay(2000);
    for(int a=0; a<=20; a+=1){
        srv_yaw.write(zero[11]-20+a);
        delay(50);
    }
    srv_l_eye_lids.write(zero[2]);
    srv_r_eye_lids.write(zero[7]);
    delay(1000);
}

void setup() { 
    srv_l_eye_v_ball.attach(4);
    srv_l_eye_h_ball.attach(5);
    srv_l_eye_lids.attach(10);
    //srv_l_brow.attach(12);
    srv_l_lips.attach(6);

    srv_r_eye_v_ball.attach(3);
    srv_r_eye_h_ball.attach(2);
    srv_r_eye_lids.attach(11);
    //srv_r_brow.attach(9);
    srv_r_lips.attach(7);

    srv_mouth.attach(8);
    srv_yaw.attach(1);
    srv_pitch.attach(0);
    
    set_zeros();
    reset();
}

void loop() {
    random_idle();
    //blinking_down();
    //delay(1000);
} 
