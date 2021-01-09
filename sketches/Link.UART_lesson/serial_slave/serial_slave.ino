byte ledPin = 13;
byte ledState = 0;
byte v;

void setup() {
    Serial.begin(9600);
    pinMode( ledPin, OUTPUT );
}

void loop() {
    if( Serial.available() ){
        v = Serial.read();
        digitalWrite( ledPin, ledState );
        ledState = !ledState;
    }
}
