#include "Matrix3216.h"

//#define SERIAL_INPUT

#define MODE_GAME 0
#define MODE_END 1
#define MODE_RESET 2
#define MODE_READY 3

#define FRAME_GAME 0
#define FRAME_END 1

// R,G,B, A,B,C, OE,ST,CLK
Matrix3216 mtx(PD2,PD3,PD4, PB0,PB1,PB2, PD7,PD6,PD5);

int pinCTRL[4] = {PC0,PC1,PC2,PC3};  

unsigned long time, ctrl_time, update_time, reset_time;
const unsigned int ctrl_timeout = 50;
const unsigned int reset_timeout = 3000;
unsigned int update_timeout = 300;

int xinc = -1;
int yinc = 0;
byte mode = MODE_GAME;

byte snake[64][2];
byte snake_size = 1;

// random food sequence data
byte food_seq_32[16] = {24,25,28,24,21,13,27,1,16,11,9,19,21,10,28,30};
byte food_seq_16[16] = {9,11,10,10,1,12,15,4,13,8,11,6,14,8,10,3};
byte food_seq_n = 0;

// food coords
byte food[2];

// image data
byte frame[2][256] = {{
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 
},{
B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, 
B00100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00010000, B10010000, B00000000, B10000000, B10000000, B00000000, B10000000, B00000000, B10010000, B00000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00000000, B10000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B10010000, B00010000, B00000000, B00010000, B00010000, B00010000, B00000000, B00000000, B10000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B00010000, B00010000, B00000000, B00010000, B00010000, B10010000, B00010000, B10000000, B10000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00010000, B10010000, B00010000, B00000000, B00010000, B00010000, B00010000, B00000000, B10010000, B00000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B00010000, B10010000, B00010000, B10010000, B00010000, B00010000, B00000000, B10010000, B00000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B10000000, B00010000, B00000000, B00010000, B00010000, B00010000, B00010000, B00000000, B10000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B00010000, B10010000, B00000000, B10010000, B00010000, B00010000, B00010000, B00000000, B10000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B10000000, B00010000, B00000000, B00010000, B00010000, B00010000, B00010000, B00000000, B10000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B10000000, B00010000, B00010000, B10010000, B00000000, B10000000, B00000000, B10010000, B00000000, B00000000, B00000000, B00000100, 
B00100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000100, 
B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100, B00100100 
}};


void setup() {
    #ifdef SERIAL_INPUT
    Serial.begin(115200);
    #else
    // set button pins to input
    for(int i=0; i<4; i++){
        DDRC &= ~_BV(pinCTRL[i]);
    }
    #endif
    
    mtx.setup();
    mtx.setFrame(&(frame[FRAME_GAME][0]));
    resetGame();
}

void loop() {
    byte x, y, z, rise_snake;

    time = millis();
    if( mode == MODE_END ){
        reset_time = time;
        mode = MODE_RESET;
    }

    if( mode == MODE_RESET && time > reset_time + reset_timeout ){
        mtx.setFrame(&(frame[FRAME_END][0]));
        mode = MODE_READY;
    }

    if( time > ctrl_time + ctrl_timeout ){
        ctrl_time = time;

        #ifdef SERIAL_INPUT
        while( Serial.available() ){
            if( mode == MODE_READY ){
                mode = MODE_GAME;
                resetGame();
            }
            switch (Serial.read()){
                case 'w':{
                    xinc = 0;
                    yinc = -1;
                    break;
                }
                case 's':{
                    xinc = 0;
                    yinc = 1;
                    break;
                }
                case 'a':{
                    yinc = 0;
                    xinc = 1;
                    break;
                }
                case 'd':{
                    yinc = 0;
                    xinc = -1;
                    break;
                }
            }
        }
        #else
        if( PINC & _BV(pinCTRL[0]) ){
            xinc = 1;
            yinc = 0;
        } else if( PINC & _BV(pinCTRL[1]) ){
            xinc = -1;
            yinc = 0;
        } else if( PINC & _BV(pinCTRL[2]) ){
            xinc = 0;
            yinc = 1;
        } else if( PINC & _BV(pinCTRL[3]) ){
            if( mode == MODE_READY ){
                mode = MODE_GAME;
                resetGame();
            }
            xinc = 0;
            yinc = -1;
        }   
        #endif
    }

    if( mode == MODE_GAME && time > update_time + update_timeout ){
        update_time = time;

        x = snake[0][0] + xinc;
        y = snake[0][1] + yinc;

        // move snake tail
        for( z=snake_size; z!=0; z-- ){
            snake[z][0] = snake[z-1][0];
            snake[z][1] = snake[z-1][1];
            // check snake self intersection
            if( x==snake[z-1][0] && y==snake[z-1][1] )
                mode = MODE_END;
        }

        snake[0][0] = x;
        snake[0][1] = y;
        
        // check walls
        if(snake[0][0] == 31 || snake[0][0] == 0 || snake[0][1] == 15 || snake[0][1] == 0){
            mode = MODE_END;

        // draw snake
        } else {
            // rise snake on feeding
            rise_snake = 0;
            if( snake[0][0]==food[0] && snake[0][1]==food[1] ){
                mtx.clearPixel(food[0], food[1]);
                food[0] = getRandomFood(32);
                food[1] = getRandomFood(16);
                mtx.setPixel(food[0], food[1], COLOR_RED);

                rise_snake = 1;
                snake_size++;
                // speed up snake
                update_timeout = 300 - 20*(snake_size>>2);
            }

            // clear last tail element
            if( !rise_snake ){
                mtx.clearPixel(snake[snake_size][0], snake[snake_size][1], COLOR_GREEN);
            }

            // draw snake head
            mtx.setPixel(snake[0][0], snake[0][1], COLOR_GREEN);
        }
    }
    mtx.drawFrame();
}

byte getRandomFood(byte val){
    if(food_seq_n == 16)
        food_seq_n = 0;
    if(val==32){
        return food_seq_32[food_seq_n++];
    }
    else if(val==16){
        return food_seq_16[food_seq_n++];
    }
}

void resetGame(){
    byte x,y;
    mtx.setFrame(&(frame[FRAME_GAME][0]));
    // clear field
    for( x=1; x<31; x++ )
        for( y=1; y<15; y++ )
            mtx.clearPixel(x, y);

    for( x=0; x<32; x++ ){
        mtx.setPixel(x, 0, COLOR_BLUE);
        mtx.setPixel(x, 15, COLOR_BLUE);
    }
    for( y=1; y<15; y++ ){
        mtx.setPixel(0, y, COLOR_BLUE);
        mtx.setPixel(31, y, COLOR_BLUE);
    }
    // set start length and position
    snake_size = 1;
    snake[0][0] = 28;
    snake[0][1] = 8;

    // set food    
    food_seq_n = 0;
    food[0] = getRandomFood(32);
    food[1] = getRandomFood(16);
    mtx.setPixel(food[0], food[1], COLOR_RED);
}


