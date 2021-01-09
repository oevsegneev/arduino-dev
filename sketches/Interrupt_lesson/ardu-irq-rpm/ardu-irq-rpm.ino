const byte interruptPin = 2;
unsigned int rot = 0;
unsigned long int tm;
unsigned long int spd = 0;
unsigned int dt = 0;


void detect() {
    rot++;
    dt = millis() - tm;
    if( dt >= 100 ){
        spd = rot*60000/dt;
        rot = 0;
        tm = millis();
    }
}

void setup() {
    Serial.begin(9600);
  
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), detect, RISING);

    tm = millis();
}

void loop() {
    Serial.println(spd);
    delay(100);
}
