#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <SerialFlow.h>

SerialFlow rd(12, 11); // RX, TX
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define BAUD_RATE 19200

#define LPF_1 0.1 // Low pass coeff for slow thrust, tunea, tune b
#define LPF_2 0.2 // Low pass coeff for fast pitch and roll

#define CMD_CTRL 0xA1
#define CMD_EN 0xA2
#define CMD_TELE 0xA3
#define ACK_CTRL 0xB1
#define ACK_EN 0xB2
#define ACK_TELE 0xB3

#define ERR_INIT_THR 0x1
#define ERR_INIT_EN 0x2

#define READ_DELAY 20
#define CTRL_DELAY 50
#define LCD_DELAY 100
#define TELE_DELAY 1000
#define ENSW_DELAY 100
#define ENSW_BURST 5

#define MIN_THRUST 5

#define ST_NONE 0 
#define ST_SEND_EN 1
#define ST_ACK_EN 2
#define ST_SEND_CTRL 3 
#define ST_ACK_CTRL 4 
#define ST_SEND_TELE 5
#define ST_ACK_TELE 6

#define MAX_WAIT_ACK 10

#define PITCH_ZERO 512
#define ROLL_ZERO 516

const int led = 13;
const byte RollPin = A7;
const byte PitchPin = A6;
const byte TuneCPin = A0;
const byte TuneDPin = A1;
const byte ThrPin = A2;
const byte TuneAPin = A3;
const byte TuneBPin = A4;
const byte EnSwPin = 10;

short thr_raw, pitch_raw, roll_raw, tunea_raw, tuneb_raw, tunec_raw, tuned_raw;
short thr, pitch, roll, tunea, tuneb, tunec, tuned;

short sf_v[8];
char sf_idx = 0;

unsigned int time;
unsigned int read_time, ctrl_time, ensw_time, lcd_time, tele_time;
boolean ensw_raw, ensw_check_value, ensw_check_state;
boolean enabled;
int error;
byte rstate = ST_NONE;
byte wait_ack = 0;

short cmd, val, tele_1, tele_2;

void setup() {
    //pc.begin(BAUD_RATE);  
    lcd.begin(16, 2);
    pinMode(EnSwPin, INPUT); 
    pinMode(led, OUTPUT);   

    rd.setPacketFormat(SerialFlow::COMPLEX, 2, 8);
    rd.begin(BAUD_RATE);
    check_init();
}

void read_ctrl(){
    ensw_raw = digitalRead(EnSwPin);            
    thr_raw = analogRead(ThrPin);            
    pitch_raw = analogRead(PitchPin);            
    roll_raw = analogRead(RollPin);            
    tunea_raw = analogRead(TuneAPin);            
    tuneb_raw = analogRead(TuneBPin);            
    tunec_raw = analogRead(TuneCPin);            
    tuned_raw = analogRead(TuneDPin);
    
    // low pass filter
    thr = (1-LPF_1)*thr + LPF_1*thr_raw/10;
    tunea = (1-LPF_1)*tunea + LPF_1*tunea_raw/10;
    tuneb = (1-LPF_1)*tuneb + LPF_1*int(tuneb_raw * 100.0/1023.0) * 10;
    tunec = (1-LPF_2)*tunec + LPF_2*tunec_raw/5;
    tuned = (1-LPF_2)*tuned + LPF_2*tuned_raw/5;

    pitch = (1-LPF_2)*pitch + LPF_2*(pitch_raw - PITCH_ZERO) * 30/1023.0;
    roll = (1-LPF_2)*roll + LPF_2*(roll_raw - ROLL_ZERO) * 30/1023.0;
}

void send_ctrl(){
    if( rstate == ST_NONE || rstate == ST_ACK_CTRL ){
        // send thr
        rd.setPacketValue(CMD_CTRL);
        rd.setPacketValue(thr);
        rd.setPacketValue(pitch);
        rd.setPacketValue(roll);
        rd.setPacketValue(tunea);
        rd.setPacketValue(tuneb);
        rd.setPacketValue(tunec);
        rd.setPacketValue(tuned);
        rd.sendPacket();
        rstate = ST_ACK_CTRL;
    }
    else if( rstate == ST_SEND_EN || rstate == ST_ACK_EN ){
        rd.setPacketValue(CMD_EN);
        rd.setPacketValue(ensw_check_value);
        rd.sendPacket();
        rstate = ST_ACK_EN;
    }        
    else if( rstate == ST_SEND_TELE || rstate == ST_ACK_TELE ){
        rd.setPacketValue(CMD_TELE);
        rd.setPacketValue(0x1);
        rd.sendPacket();
        rstate = ST_ACK_TELE;
    } 
}

void lcd_out(){
    lcd.clear();
    lcd.setCursor(15, 0);
    if( rstate == ST_ACK_EN )
       lcd.print("W");
    else if( enabled )
       lcd.print("E");
    else    
       lcd.print("D");

    lcd.setCursor(0, 0);
    lcd.print(thr);

    lcd.setCursor(4, 0);
    lcd.print(tunea);

    lcd.setCursor(8, 0);
    lcd.print(tuneb);

    lcd.setCursor(0, 1);
    lcd.print(tele_1);

    lcd.setCursor(4, 1);
    lcd.print(tele_2);

    lcd.setCursor(8, 1);
    lcd.print(roll);

    lcd.setCursor(12, 1);
    lcd.print(pitch);
}

// Before run copter is needed to: 
// drop thrust to 0
// and turn off power switch
void check_init(){
    thr_raw = analogRead(ThrPin);            
    ensw_raw = digitalRead(EnSwPin);
    
    lcd.clear();
    if( thr_raw > MIN_THRUST ){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Init error:");
        lcd.setCursor(0, 1);
        lcd.print("Positive thrust");
        error |= ERR_INIT_THR;
        return;
    }
    if( ensw_raw ){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Init error:");
        lcd.setCursor(0, 1);
        lcd.print("Enabled");
        error |= ERR_INIT_EN;
        return;
    }
    error = 0;
}

void loop() {
    while( error ){
        check_init();
        delay(100);
        return;
    }

    // read ctrl from inputs
    time = millis();
    if( time - read_time > READ_DELAY ){
        read_time = time;
        read_ctrl();
    }   

    // receive telemetry
    while( rd.receivePacket() ){
        cmd = rd.getPacket(0);
        if( rstate == ST_ACK_EN && cmd == ACK_EN ){
            enabled = ensw_check_value;
            if( enabled )
                digitalWrite(led, HIGH);
            else
                digitalWrite(led, LOW);
            rstate = ST_NONE;
        }
        else if( rstate == ST_ACK_CTRL && cmd == ACK_CTRL ){
            rstate = ST_NONE;
        }
        else if( rstate == ST_ACK_TELE && cmd == ACK_TELE ){
            tele_1 = rd.getPacket(1);
            tele_2 = rd.getPacket(2);
            rstate = ST_NONE;
        }
    }

    time = millis();
    if( ensw_raw != ensw_check_value ){
        ensw_time = time;
        ensw_check_value = ensw_raw;
        ensw_check_state = 1;
    }

    // check enable timeout
    if( ensw_check_state && (time-ensw_time > ENSW_DELAY) ){
        ensw_check_state = 0;
        if( ensw_check_value == ensw_raw )
            rstate = ST_SEND_EN;
    }

    // check tele timeout
    time = millis();
    if( enabled && rstate == ST_NONE && time - tele_time > TELE_DELAY ){
        tele_time = time;
        rstate = ST_SEND_TELE;
    }

    // send ctrl
    time = millis();
    if( time - ctrl_time > CTRL_DELAY ){
        ctrl_time = time;
        if( rstate == ST_SEND_EN || enabled && ( rstate == ST_NONE || rstate == ST_SEND_TELE )){
            send_ctrl();
            wait_ack = 0;
        } else if( rstate != ST_NONE && (wait_ack++ > MAX_WAIT_ACK )){
            send_ctrl();
            wait_ack = 0;
        }
    }
    
    time = millis();
    if( time - lcd_time > LCD_DELAY){
        lcd_time = time;
        lcd_out();
    }
}
