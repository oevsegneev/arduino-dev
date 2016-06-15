#include <PinChangeInt.h>
#include <SerialFlow.h>

//#define DEBUGM 1

#define BAUD_RATE 19200

#define HOLES 20
#define FORWARD 0
#define BACKWARD 1
#define RELEASE 2
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1
#define STALL_TPULSE 50000

#define CMD_CTRL 0xA1
#define ACK_CTRL 0xB1
#define CTRL_DELAY 10
#define ST_SEND_CTRL 0 
#define ST_ACK_CTRL 1

#define MOTOR_TO 999
#define TELE_TO 100

const int mpin[2][2] = {{3,6},{9,10}}; // In1, In2, In3 и In4 двигателей
const int p1pin = A0; // датчик 1
const int p2pin = A1; // датчик 2
const int e1pin = 7; // энкодер 1
const int e2pin = 8; // энкодер 2
const int bpin = 13;

const int pk = 2; // коэффициент пропорционального управления

uint8_t dir, val, cmd;
int p1_val, p2_val;
uint32_t tpulse = 40000;
uint8_t rdir, ldir, stopped;
uint8_t pwm[2];

uint32_t lspd, rspd;

uint32_t mtime = 0;
uint32_t ttime = 0;
#ifdef DEBUGM
uint32_t dtime = 0;
#endif

volatile uint32_t lpulse = 100000;
volatile uint32_t rpulse = 100000;
volatile unsigned int lcount, rcount;

volatile uint32_t ltime;
volatile uint32_t rtime;

SerialFlow rd(&Serial); // RX, TX

void beep(int n, int d){
    for(int i=0; i<n; i++){
        digitalWrite(bpin, HIGH);
        delay(d);
        digitalWrite(bpin, LOW);
        delay(d);
    }
}

void sendTele(){
    rd.setPacketValue(lcount);
    rd.setPacketValue(p1_val);
    rd.setPacketValue(p2_val);
    rd.sendPacket();
}

void setup() {
    #ifndef DEBUGM
    rd.setPacketFormat(2, 3, 1);
    #else
    rd.setPacketFormat(1, 3, 0);
    #endif
    rd.begin(BAUD_RATE);
    
    pinMode(bpin, OUTPUT);

    pinMode(mpin[0][0], OUTPUT);
    pinMode(mpin[0][1], OUTPUT);
    pinMode(mpin[1][0], OUTPUT);
    pinMode(mpin[1][1], OUTPUT);

    pinMode(p1pin, INPUT);
    pinMode(p2pin, INPUT);

    pinMode(e1pin, INPUT_PULLUP);
    attachPinChangeInterrupt(e1pin, Lencoder, RISING);
    pinMode(e2pin, INPUT_PULLUP);
    attachPinChangeInterrupt(e2pin, Rencoder, RISING);

    ldir = RELEASE;
    rdir = RELEASE;

    stopped = 1;
    lspd = 0;
    rspd = 0;

    #ifdef DEBUGM
    ldir = FORWARD;
    rdir = FORWARD;
    stopped = 0;
    #endif
    
    beep(5, 50);
}

void loop() {
    p1_val = digitalRead(p1pin);
    p2_val = analogRead(p2pin);
    // receive ctrl data
    #ifndef DEBUGM
    if( rd.receivePacket() ){
        cmd = rd.getPacketValue(0);
        if( cmd == CMD_CTRL ){
            dir = rd.getPacketValue(1);
            val = rd.getPacketValue(2);
            setMotors(dir, val);
        }
    }

    if( millis()-ttime > TELE_TO ){
        ttime = millis();
        sendTele();
    }
    #endif
    
    if( micros()-mtime > MOTOR_TO ){
        mtime = micros();
        controlMotors();
    }
}
