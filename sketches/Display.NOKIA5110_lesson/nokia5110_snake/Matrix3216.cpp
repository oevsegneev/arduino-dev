#include "Matrix3216.h"

Matrix3216::Matrix3216(int CR, int CG, int CB, int AA, int AB, int AC, int OE, int ST, int CLK){
    pinRGB[0] = CR;
    pinRGB[1] = CG;
    pinRGB[2] = CB;

    pinABC[0] = AA;
    pinABC[1] = AB;
    pinABC[2] = AC;

    pinOE = OE;
    pinST = ST;
    pinCLK = CLK;
}

void Matrix3216::setup(){
    // set rgb pins to output
    for(int i=0; i<3; i++){
        DDRD |= _BV(pinRGB[i]);
        dwrite(pinRGB[i], 0);
    }
    // set address pins to output
    for(int i=0; i<3; i++){
        DDRB |= _BV(pinABC[i]);
        bwrite(pinABC[i], 0);
    }

    DDRD |= _BV(pinOE);
    DDRD |= _BV(pinST);
    DDRD |= _BV(pinCLK);

    dwrite(pinCLK, 0);
    dwrite(pinOE, 1);
    dwrite(pinST, 0);

    dwrite(pinRGB[0], 0);
    dwrite(pinRGB[1], 0);
    dwrite(pinRGB[2], 0);

    for(int i=0; i<32; i++){
        clock();
        dwrite(pinST, 1);
        dwrite(pinST, 0);
    }  
}

void Matrix3216::setFrame(byte *frame){
    this->frame = frame;
}

void Matrix3216::addressRow(byte _address) {
    bwrite(pinABC[0], bitRead(_address, 0));//LSB
    bwrite(pinABC[1], bitRead(_address, 1));
    bwrite(pinABC[2], bitRead(_address, 2));//MSB
}

void Matrix3216::clock(){
    dwrite(pinCLK, 0);
    dwrite(pinCLK, 1);
}

void Matrix3216::dwrite(int pin, int val){
    if(val)
        PORTD |= _BV(pin);
    else    
        PORTD &= ~_BV(pin);
}

void Matrix3216::bwrite(int pin, int val){
    if(val)
        PORTB |= _BV(pin);
    else    
        PORTB &= ~_BV(pin);
}

void Matrix3216::drawFrame(){
    byte x,y,z;
    // iterate through rows, each row - 64 pixels
    for( row=0; row!=8; row++ ){
        block = 0;
        // iterate through blocks
        while( block!=8 ){
            x = trans[block][0];
            z = trans[block][1];
            y = z+7-row;
            bty = 0;
            bti = 0;
            // iterate through 4 byte = 8 pixels
            while( bty!=4 ){
                clr = 0;
                // iterate through 3 color components
                while( clr!=3 ){
                    if(z==0)
                        dwrite(pinRGB[clr], frame[(x+bty) + (y<<4)] & _BV(7-bti) );
                    else
                        dwrite(pinRGB[2-clr], frame[(x+3-bty) + (y<<4)] & _BV(2+bti) );
                    if( bti==5 ){
                        bti = 0;
                        bty++;
                    } else
                        bti++;
                    clr++;
                }    
                clock();
            }
            block++;        
        }

        dwrite(pinST, 1);
        dwrite(pinST, 0);
    
        //clock();
        dwrite(pinOE, 1);
        addressRow(row);
        dwrite(pinOE, 0);
    } 
}

void Matrix3216::clearPixel(byte x, byte y){
    if( x&1 )
        frame[(x>>1) + (y<<4)] &= ~(7<<2); // if second triad
    else
        frame[(x>>1) + (y<<4)] &= ~(7<<5); // if first triad
}

void Matrix3216::clearPixel(byte x, byte y, byte color){
    if( x&1 )
        frame[(x>>1) + (y<<4)] &= ~_BV(color-3); // if second triad
    else
        frame[(x>>1) + (y<<4)] &= ~_BV(color); // if first triad
}

void Matrix3216::setPixel(byte x, byte y, byte color){
    if( x&1 )
        frame[(x>>1) + (y<<4)] |= _BV(color-3); // if second triad
    else
        frame[(x>>1) + (y<<4)] |= _BV(color); // if first triad
}
