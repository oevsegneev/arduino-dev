const byte data_pin = PD2;
const byte st_pin = PD3;
const byte sh_pin = PD4;
const byte oe_pin = PD5;

unsigned long tm, next_flick, next_switch;
const unsigned int to_flick = 500;
const unsigned long to_switch = 500000;

byte line = 0;
byte frame = 0;

const byte data[2][8] = {
{ 0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
},
{ 0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10000001,
  0b10111101,
  0b01000010,
  0b00111100  
}};

void latchOn(){
    digitalWriteFast(st_pin, HIGH); 
    digitalWriteFast(st_pin, LOW);
}

void fill( byte d ){
    for(char i=0; i<8; i++){
        digitalWriteFast(sh_pin, LOW);
        digitalWriteFast(data_pin, d & (1<<i));
        digitalWriteFast(sh_pin, HIGH);
    }
}

void setPinFast(byte pin){
    DDRD |= _BV(pin);
}

void digitalWriteFast(byte pin, byte sig){
  if( sig )
    PORTD |= _BV(pin);
  else
    PORTD &= ~_BV(pin);
}

void setup() {
    setPinFast(data_pin);
    setPinFast(st_pin);
    setPinFast(sh_pin);
    setPinFast(oe_pin);
    digitalWriteFast(oe_pin, LOW);
}

void loop() {
    tm = micros();
    if( tm > next_flick ){
        next_flick = tm + to_flick;
        line++;
        if( line == 8 )
            line = 0;
        // включаем строку № line
        fill( ~(1<<(7-line)) );
        // зажигаем точки в строке № line
        fill( data[frame][7-line] );
        latchOn();
    }
    
    tm = micros();
    if( tm > next_switch ){
        next_switch = tm + to_switch;
        frame = !frame;
    }
}
