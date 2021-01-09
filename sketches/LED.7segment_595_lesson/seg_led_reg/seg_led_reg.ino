const int data_pin = 2;
const int sh_pin = 3;
const int st_pin = 4;

int bt = 0;

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
    shift(1<<bt);
    bt++;
    if( bt==8 )
        bt = 0;
    delay(200);
}
