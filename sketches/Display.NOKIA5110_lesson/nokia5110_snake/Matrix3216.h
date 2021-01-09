#ifndef _Matrix3216_h_
#define _Matrix3216_h_

#include <arduino.h>
#include <inttypes.h>
#include <avr/io.h>

#define COLOR_RED 7
#define COLOR_GREEN 6
#define COLOR_BLUE 5
#define COLOR_BLACK 0

// block map
const byte trans[8][2] = {{0,8},{0,0},{4,8},{4,0},
                    {8,8},{8,0},{12,8},{12,0}};

class Matrix3216 {
    public:
        Matrix3216(int CR, int CG, int CB, int AA, int AB, int AC, int OE, int ST, int CLK);
        void setup();
        void setFrame(byte *frame);
        void drawFrame();
        void clearPixel(byte x, byte y);
        void clearPixel(byte x, byte y, byte color);
        void setPixel(byte x, byte y, byte color);
    
    private:
        byte *frame;
        int pinRGB[3]; 
        int pinABC[3];  

        int pinOE = PD7;
        int pinST = PD6;
        int pinCLK = PD5;

        int row,block,bty,clr,bti;
        void dwrite(int pin, int val);
        void bwrite(int pin, int val);
        void addressRow(byte _address);
        void clock();
};

#endif
