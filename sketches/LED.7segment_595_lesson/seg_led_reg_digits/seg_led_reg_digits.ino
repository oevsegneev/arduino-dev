const int data_pin = 2;
const int sh_pin = 4;
const int st_pin = 3;

int bt = 0;

byte digits[10] = {0b00111111, // 0
                   0b00000110, // 1
                   0b01011011, // 2
                   0b01001111, // 3
                   0b01100110, // 4
                   0b01101101, // 5
                   0b01111101, // 6
                   0b00000111, // 7
                   0b01111111, // 8
                   0b01101111, // 9
                   };

void shift(byte v){
    for(int i=0; i<8; i++){
        digitalWrite(sh_pin, LOW);
        digitalWrite(data_pin, v & (1 << i ));
        digitalWrite(sh_pin, HIGH);
    }
    digitalWrite(st_pin, HIGH); 
    digitalWrite(st_pin, LOW);
}

void setup() {
    pinMode(data_pin, OUTPUT);
    pinMode(sh_pin, OUTPUT);
    pinMode(st_pin, OUTPUT);
}

void loop() {
    shift( digits[bt] );
    bt++;
    if( bt==10 )
        bt = 0;
    delay(500);
}
