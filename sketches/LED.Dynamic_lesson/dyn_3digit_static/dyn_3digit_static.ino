const byte digit_pins[3] = {5,6,7};
const byte data_pin = 2;
const byte sh_pin = 3;
const byte st_pin = 4;

unsigned long tm, next_flick;
const unsigned int to_flick = 1;

byte digit = 0;
unsigned int counter = 125;

const byte digits[10] = {
  B11101110, 
  B10000010, 
  B11011100, 
  B11010110, 
  B10110010, 
  B01110110, 
  B01111110, 
  B11000010, 
  B11111110, 
  B11110110 
};

void fill( byte d ){
    for(char i=0; i<8; i++){
        digitalWrite(sh_pin, LOW);
        digitalWrite(data_pin, digits[d] & (1<<i));
        digitalWrite(sh_pin, HIGH);
    }
    digitalWrite(st_pin, HIGH); 
    digitalWrite(st_pin, LOW);
}

void setDigit( byte digit, unsigned int counter ){
    byte d = 0;
    switch ( digit ){
        case 0:
            digitalWrite(digit_pins[2], LOW);
            d = counter % 10;
            fill(d);
            digitalWrite(digit_pins[0], HIGH);
            break;
        case 1:
            digitalWrite(digit_pins[0], LOW);
            d = (counter % 100) / 10;
            fill(d);
            digitalWrite(digit_pins[1], HIGH);
            break;
        case 2:
            digitalWrite(digit_pins[1], LOW);
            d = ( counter % 1000 ) / 100;
            fill(d);
            digitalWrite(digit_pins[2], HIGH);
            break;
    }
}

void setup() {
    for(int i=0; i<3; i++){
        pinMode(digit_pins[i], OUTPUT);
    }
    pinMode(data_pin, OUTPUT);
    pinMode(sh_pin, OUTPUT);
    pinMode(st_pin, OUTPUT);

    pinMode(13, OUTPUT);
}

void loop() {
    tm = millis();
    if( tm > next_flick ){
        next_flick = tm + to_flick;
        digit++;
        if( digit == 3 )
            digit = 0;
        setDigit( digit, counter );
    }
}
