byte ledPin = 3;

void setup() {
    Serial.begin(9600);
    pinMode( ledPin, OUTPUT );
}

void loop() {
    if( Serial.available() ){
        byte v = Serial.read();
        analogWrite( ledPin, v);
    }
}
