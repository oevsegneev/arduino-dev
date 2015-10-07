#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#define isPAL 1

#include <FastSerial.h>
#include <SerialFlowFast.h>
#include <AP_Common.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "ArduCam_Max7456.h"

OSD osd;
FastSerialPort0(Serial);
SerialFlowFast rd;

//#define MAX7456_screen_size 480 //15x32

#define TELE_RATE 57600

#define TELEQ 8
#define PANQ 8
#define DELAY_PAN 100

#define PAN_OFF 0
#define PAN_BSF 1 // basic float
#define PAN_BSI 2 // basic integer
#define PAN_CMP 3 // compass
#define PAN_MOD 4 // mode

// modes
#define MODE_ON 1
#define MODE_MANUAL 2
#define MODE_READY 4
#define MODE_TAKEOFF 8
#define MODE_STAB_ALT 16
#define MODE_STAB_PRY 32

#define CMD_TELE 0xA3

typedef struct{
  byte tp;
  byte x,y;
  byte icon;
  byte postfix;
} TPan;

TPan pans[PANQ] = {{PAN_BSI, 1,1,  0x17, 0x25},  //battery charge
                   {PAN_BSI, 1,2,  0x02, 0x25},  //average thrust
                   {PAN_BSF, 1,4,  0x12, 0xff},  //imu alt
                   {PAN_BSF, 1,5,  0xff, 0xff},  //target alt
                   {PAN_CMP, 19,13,0xff, 0xff},  //compass heading
                   {PAN_BSI, 1,7,  0x07, 0x05},  //roll
                   {PAN_BSI, 1,8,  0x06, 0x05},  //pitch
                   {PAN_MOD, 1,14, 0x7f, 0xff},  //mode
                  };
               
int data[TELEQ];

void drawPan(TPan pan, int value, int dv);
void drawCmp(TPan pan, int value);
void drawMod(TPan pan, int value);

void drawPan(TPan pan, int value, int dv){
    osd.setPanel(pan.x,pan.y);
    osd.openPanel();
    if(pan.icon != 0xff)
        osd.printf("%c", pan.icon);

    if(dv != 0)
        osd.printf("%.2f", value/float(dv)); 
    else
        osd.printf("%d", value); 
    
    if(pan.postfix != 0xff)
        osd.printf("%c", pan.postfix);
    osd.closePanel();  
}

static byte cmp_row[24]={0x82,0x80,0x81,0x80,0x81,0x80,
                         0x84,0x80,0x81,0x80,0x81,0x80,
                         0x83,0x80,0x81,0x80,0x81,0x80,
                         0x85,0x80,0x81,0x80,0x81,0x80};
                         
void drawCmp(TPan pan, int value){
    char k;
    char center = value/15;
    osd.setPanel(pan.x+1,pan.y);
    osd.openPanel();
    osd.printf("%d", value);
    osd.closePanel();  

    osd.setPanel(pan.x,pan.y+1);
    osd.openPanel();
    osd.printf("%c", 0xc3);
    k = center - 3;
    for( char i=0; i<7; i++ ){
        if( k<0 ) k += 24;
        if( k>23 ) k -= 24;
        osd.printf("%c", cmp_row[k]); 
        k++;
    }  
    osd.printf("%c", 0x87); 
    osd.closePanel();  
}

void drawMod(TPan pan, int value){
    osd.openPanel();
    osd.setPanel(pan.x,pan.y);    
    osd.printf("%c", pan.icon);
    if( value & MODE_ON )
        osd.printf_P(PSTR("ON.")); 
    if( value & MODE_TAKEOFF )
        osd.printf_P(PSTR("TO.")); 
    if( value & MODE_STAB_ALT )
        osd.printf_P(PSTR("SA.")); 
    if( value & MODE_STAB_PRY )
        osd.printf_P(PSTR("SP.")); 
    osd.closePanel();  
}

void drawHello(){
    osd.setPanel(6,7);
    osd.openPanel();
    osd.printf_P(PSTR("MakeItLab OSD v.0.1")); 
    osd.closePanel();  
}

void drawPans(){
    osd.clear();
    for(byte i = 0; i<PANQ; i++){
        switch (pans[i].tp){
            case PAN_OFF:
                continue;
            case PAN_BSI:
                drawPan(pans[i], data[i], 0);
                break;
            case PAN_BSF:
                drawPan(pans[i], data[i], 100);
                break;
            case PAN_CMP:    
                drawCmp(pans[i], data[i]);
                break;
            case PAN_MOD:    
                drawMod(pans[i], data[i]);
                break;
            default:
                break;
        }
    }
}  

void setup() {
    Serial.begin(TELE_RATE);
    rd.setPort(&Serial);
    rd.setPacketFormat(SerialFlowFast::COMPLEX, 2, 9);

    pinMode(MAX7456_SELECT,  OUTPUT); // OSD CS  
    digitalWrite(MAX7456_SELECT,  HIGH); // unplug OSD
    osd.init();
    delay(500);

    drawHello();
    delay(2000);
}

void loop() {
    uint16_t cmd;
  
    while( rd.receivePacket() ){
        cmd = rd.getPacket(0);
        if( cmd == CMD_TELE ){
            for(byte i=0; i<PANQ; i++){
                data[i] = rd.getPacket(i+1);
            }
            drawPans();
        }
    }    
}
