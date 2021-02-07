byte photoPin = A0;
byte anPins[5] = {A1,A2,A3,A4,A5};

void setup() {
    Serial.begin(9600);
    pinMode( photoPin, INPUT );
    for(byte i=0; i<5; i++){
      pinMode( anPins[i], OUTPUT );
      digitalWrite( anPins[i], LOW );
    }
}

void loop() {
    int v = analogRead(photoPin);
    Serial.println(v);
    delay(100);
}
